#include "SubmarineFree.hpp"
#include "torpedo.hpp"

struct TF_101 : Module  {
	enum ParamIds {
		PARAM_FG_RED,
		PARAM_FG_GREEN,
		PARAM_FG_BLUE,
		PARAM_BG_RED,
		PARAM_BG_GREEN,
		PARAM_BG_BLUE,
		PARAM_FONT_SIZE,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT_FG_RED,
		INPUT_FG_GREEN,
		INPUT_FG_BLUE,
		INPUT_BG_RED,
		INPUT_BG_GREEN,
		INPUT_BG_BLUE,
		INPUT_FONT_SIZE,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_TOR,
		NUM_OUTPUTS
	};
	enum LightIds {
		LIGHT_FG_RED,
		LIGHT_FG_GREEN,
		LIGHT_FG_BLUE,
		LIGHT_BG_RED,
		LIGHT_BG_GREEN,
		LIGHT_BG_BLUE,
		NUM_LIGHTS
	};

	float prevValues[7];
	int isDirty = false;
	TF_101() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		prevValues[0] = 0.1569f;
		prevValues[1] = 0.6902f;
		prevValues[2] = 0.9529f;
		prevValues[6] = 12.0f;
	}
	Torpedo::PatchOutputPort outPort = Torpedo::PatchOutputPort(this, OUTPUT_TOR);	
	void step() override;
	std::string encodeColor(float r, float g, float b) {
		std::string out;
		out.push_back('A'+(int)(r * 255) / 16);	
		out.push_back('A'+(int)(r * 255) % 16);	
		out.push_back('A'+(int)(g * 255) / 16);	
		out.push_back('A'+(int)(g * 255) % 16);	
		out.push_back('A'+(int)(b * 255) / 16);	
		out.push_back('A'+(int)(b * 255) % 16);	
	
		return out;
	}
};

void TF_101::step() {
	for (int i = 0; i < 6; i++) {
		float newValue = clamp(params[PARAM_FG_RED + i].value + inputs[INPUT_FG_RED + i].value / 10.0f, 0.0f, 1.0f); 
		lights[LIGHT_FG_RED + i].value = newValue; 
		if (prevValues[i] != newValue) {
			isDirty = true;
			prevValues[i] = newValue;
		}
	}
	float newValue = clamp(params[PARAM_FONT_SIZE].value + inputs[INPUT_FONT_SIZE].value * 2.0f, 6.0f, 26.0f); 
	if (prevValues[6] != newValue) {
		isDirty = true;
		prevValues[6] = newValue;
	}
	if (isDirty) {
		isDirty = false;
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "fg", json_string(encodeColor(prevValues[0], prevValues[1], prevValues[2]).c_str()));
		json_object_set_new(rootJ, "bg", json_string(encodeColor(prevValues[3], prevValues[4], prevValues[5]).c_str()));
		json_object_set_new(rootJ, "size", json_real(prevValues[6]));
		outPort.send("SubmarineFree", "TDNotesColor", rootJ);
	}
	outPort.process();
}

struct WhiteLight : GrayModuleLightWidget {
	WhiteLight() {
		addBaseColor(nvgRGB(0xff, 0x00, 0x00));
		addBaseColor(nvgRGB(0x00, 0xff, 0x00));
		addBaseColor(nvgRGB(0x00, 0x00, 0xff));
	}
};


struct TF101 : ModuleWidget {
	TF101(TF_101 *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/AG-106.svg")));

		addInput(Port::create<sub_port>(Vec(4,25), Port::INPUT, module, TF_101::INPUT_FG_RED));
		addInput(Port::create<sub_port>(Vec(4,60), Port::INPUT, module, TF_101::INPUT_FG_GREEN));
		addInput(Port::create<sub_port>(Vec(4,95), Port::INPUT, module, TF_101::INPUT_FG_BLUE));
		addInput(Port::create<sub_port>(Vec(4,135), Port::INPUT, module, TF_101::INPUT_BG_RED));
		addInput(Port::create<sub_port>(Vec(4,170), Port::INPUT, module, TF_101::INPUT_BG_GREEN));
		addInput(Port::create<sub_port>(Vec(4,205), Port::INPUT, module, TF_101::INPUT_BG_BLUE));
		addInput(Port::create<sub_port>(Vec(4,245), Port::INPUT, module, TF_101::INPUT_FONT_SIZE));

		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 25.5), module, TF_101::PARAM_FG_RED, 0.0f, 1.0f, 0.1569f));
		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 60.5), module, TF_101::PARAM_FG_GREEN, 0.0f, 1.0f, 0.6902f));
		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 95.5), module, TF_101::PARAM_FG_BLUE, 0.0f, 1.0f, 0.9529f));
		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 135.5), module, TF_101::PARAM_BG_RED, 0.0f, 1.0f, 0.0f));
		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 170.5), module, TF_101::PARAM_BG_GREEN, 0.0f, 1.0f, 0.0f));
		addParam(ParamWidget::create<sub_knob_small>(Vec(33, 205.5), module, TF_101::PARAM_BG_BLUE, 0.0f, 1.0f, 0.0f));
		addParam(ParamWidget::create<sub_knob_small_snap>(Vec(33, 245.5), module, TF_101::PARAM_FONT_SIZE, 6.0f, 26.0f, 12.0f));

		addChild(ModuleLightWidget::create<MediumLight<WhiteLight>>(Vec(66, 63), module, TF_101::LIGHT_FG_RED));
		addChild(ModuleLightWidget::create<MediumLight<WhiteLight>>(Vec(66, 173), module, TF_101::LIGHT_BG_RED));

		addOutput(Port::create<sub_port_black>(Vec(61,245), Port::OUTPUT, module, TF_101::OUTPUT_TOR));
	}
};

Model *modelTF101 = Model::create<TF_101, TF101>("SubmarineFree", "TF-101", "TF-101 Text Display Format Control", VISUAL_TAG);
