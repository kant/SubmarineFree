#include "DS.hpp"

template <int x>
struct OG_1 : DS_Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		INPUT_A_1,
		INPUT_B_1 = x,
		NUM_INPUTS = x + x
	};
	enum OutputIds {
		OUTPUT_1,
		NUM_OUTPUTS = x
	};
	enum LightIds {
		NUM_LIGHTS
	};

	OG_1() : DS_Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override {
		int setCount = 0;
		for (int i = 0; i < 6; i++) {
			if (inputs[INPUT_A_1 + i].active)
				if (inputs[INPUT_A_1 + i].value > midpoint())
					setCount++;
			if (inputs[INPUT_B_1 + i].active)
				if (inputs[INPUT_B_1 + i].value > midpoint())
					setCount++;
			if (outputs[OUTPUT_1 + i].active) {
				outputs[OUTPUT_1 + i].value = (setCount > 0)?voltage1:voltage0;
				setCount = 0;
			}
		}
	}
};

struct OG106 : ModuleWidget {
	OG106(OG_1<6> *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/OG-106.svg")));

		for (int i = 0; i < 6; i++) {
			int offset = 58 * i;
			addInput(Port::create<sub_port_blue>(Vec(4,19 + offset), Port::INPUT, module, OG_1<6>::INPUT_A_1 + i));
			addInput(Port::create<sub_port_blue>(Vec(4,47 + offset), Port::INPUT, module, OG_1<6>::INPUT_B_1 + i));

			addOutput(Port::create<sub_port_blue>(Vec(62,33 + offset), Port::OUTPUT, module, OG_1<6>::OUTPUT_1 + i));
		}
	}
	void appendContextMenu(Menu *menu) override {
		((DS_Module *)module)->appendContextMenu(menu);
	}
};

Model *modelOG106 = Model::create<OG_1<6>, OG106>("SubmarineFree", "OG-106", "OG-106 OR Gates", LOGIC_TAG, MULTIPLE_TAG);
