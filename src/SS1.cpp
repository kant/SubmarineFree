//SubTag W2 W5 W20

#include "SubmarineFree.hpp"

struct SS_112 : Module {
	static constexpr int deviceCount = 12;
	SS_112() : Module() {
		config(0, deviceCount, 0, 0);
	}
};

struct SS112 : SchemeModuleWidget {
	SS112(SS_112 *module) : SchemeModuleWidget(module) {
		this->box.size = Vec(30, 380);
		addChild(new SchemePanel(this->box.size));
		for (int i = 0; i < SS_112::deviceCount; i++) {
			addInput(createInputCentered<SilverPort>(Vec(15,31.5 + i * 29), module, i));
		}
	}
	void render(NVGcontext *vg, SchemeCanvasWidget *canvas) override {
		drawBase(vg, "SS-112");

	}
};

struct SS_208 : Module {
	static constexpr int deviceCount = 8;
	float values[8];
	SS_208() : Module() {
		config(0, 0, deviceCount, 0);
		values[0] = M_PI;
		values[1] = 2 * M_PI;
		values[2] = M_E;
		values[3] = M_SQRT1_2;
		values[4] = M_SQRT2;
		values[5] = powf(3.0f, 0.5f);
		values[6] = powf(5.0f, 0.5f);
		values[7] = powf(7.0f, 0.5f);
	}
	void process(const ProcessArgs &args) override {
		outputs[0].setVoltage(values[0]);
		outputs[1].setVoltage(values[1]);
		outputs[2].setVoltage(values[2]);
		outputs[3].setVoltage(values[3]);
		outputs[4].setVoltage(values[4]);
		outputs[5].setVoltage(values[5]);
		outputs[6].setVoltage(values[6]);
		outputs[7].setVoltage(values[7]);
	}
};

struct SS208 : SchemeModuleWidget {
	SS208(SS_208 *module) : SchemeModuleWidget(module) {
		this->box.size = Vec(30, 380);
		addChild(new SchemePanel(this->box.size));
		for (int i = 0; i < SS_208::deviceCount; i++) {
			addOutput(createOutputCentered<SilverPort>(Vec(15,31.5 + 43 * i), module, i));
		}
	}
	void render (NVGcontext *vg, SchemeCanvasWidget *canvas) override {
		drawBase(vg, "SS-208");
		drawText(vg, 15, 54, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xcf\x80");
		drawText(vg, 15, 97, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xcf\x84");
		drawText(vg, 15, 140, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x84\xaf");
		drawText(vg, 15, 183, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x88\x9a\xc2\xbd");
		drawText(vg, 15, 226, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x88\x9a" "2");
		drawText(vg, 15, 269, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x88\x9a" "3");
		drawText(vg, 15, 312, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x88\x9a" "5");
		drawText(vg, 15, 355, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 10, gScheme.getContrast(module), "\xe2\x88\x9a" "7");
	}
};

struct SS_212 : Module {
	static constexpr int deviceCount = 12;
	int v = 0;
	void setValues() {
		for (int i = 0; i < deviceCount; i++) {
			outputs[i].setVoltage(v + 1.0f * i / 12.0f);
		}
	}

	SS_212() : Module() {
		config(0, 0, deviceCount, 0);
	}
	void process(const ProcessArgs &args) override {
		setValues();
	}

	json_t *dataToJson() override {
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "octave", json_integer(v));
		return rootJ;
	}

	void dataFromJson(json_t *rootJ) override {
		json_t *intJ = json_object_get(rootJ, "octave");
		if (intJ)
			v = json_integer_value(intJ);
		setValues();
	}
};

struct SS212 : SchemeModuleWidget {
	SS212(SS_212 *module) : SchemeModuleWidget(module) {
		this->box.size = Vec(30, 380);
		addChild(new SchemePanel(this->box.size));
		for (int i = 0; i < SS_212::deviceCount; i++) {
			addOutput(createOutputCentered<SilverPort>(Vec(15,31.5 + i * 29), module, i));
		}
	}
	void appendContextMenu(Menu *menu) override {
		SchemeModuleWidget::appendContextMenu(menu);
		SS_212 *ss_212 = dynamic_cast<SS_212*>(this->module);
		if (ss_212) {
			EventWidgetMenuItem *m = createMenuItem<EventWidgetMenuItem>("Octave");
			m->rightText = SUBMENU;
			m->childMenuHandler = [=]() {
				char label[20];
				Menu *menu = new Menu();
				for (int i = -5; i < 5; i++) {
					sprintf(label, "Octave %d", i);
					EventWidgetMenuItem *menuItem = createMenuItem<EventWidgetMenuItem>(label);
					menuItem->stepHandler = [=]() {
						menuItem->rightText = CHECKMARK(ss_212->v == i);
					};
					menuItem->clickHandler = [=]() {
						ss_212->v = i;
						ss_212->setValues();
					};
					menu->addChild(menuItem);
				}
				return menu;
			};
			menu->addChild(m);
		}
	}
	void render(NVGcontext *vg, SchemeCanvasWidget *canvas) override {
		drawBase(vg, "SS-212");
		nvgFillColor(vg, gScheme.getAlternative(module));
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 17, 26, 28, 2);
		nvgRoundedRect(vg, 2, 75, 26, 28, 2);
		nvgRoundedRect(vg, 2, 133, 26, 28, 2);
		nvgRoundedRect(vg, 2, 162, 26, 28, 2);
		nvgRoundedRect(vg, 2, 220, 26, 28, 2);
		nvgRoundedRect(vg, 2, 278, 26, 28, 2);
		nvgRoundedRect(vg, 2, 336, 26, 28, 2);
		nvgFill(vg);
	}
};

struct SS_221 : Module {
	static constexpr int deviceCount = 21;
	SS_221() : Module() {
		config(0, 0, deviceCount, 0);
	}
	void process(const ProcessArgs &args) override {
		for (int i = 0; i < deviceCount; i++) {
			outputs[i].setVoltage(10.0f - i);
		}
	}
};

struct SS221 : SchemeModuleWidget {
	SS221(SS_221 *module) : SchemeModuleWidget(module) {
		this->box.size = Vec(75, 380);
		addChild(new SchemePanel(this->box.size));
		for (int i = 0; i < SS_221::deviceCount; i++) {
			addOutput(createOutputCentered<SilverPort>(Vec(15 + 45 * (i % 2),31.5 + i * 16), module, i));
		}
	}
	void render(NVGcontext *vg, SchemeCanvasWidget *canvas) override {
		drawBase(vg, "SS-221");
		drawText(vg, 37.5, 34, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "10V");
		drawText(vg, 37.5, 50, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "9V");
		drawText(vg, 37.5, 66, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "8V");
		drawText(vg, 37.5, 82, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "7V");
		drawText(vg, 37.5, 98, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "6V");
		drawText(vg, 37.5, 114, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "5V");
		drawText(vg, 37.5, 130, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "4V");
		drawText(vg, 37.5, 146, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "3V");
		drawText(vg, 37.5, 162, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "2V");
		drawText(vg, 37.5, 178, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "1V");
		drawText(vg, 37.5, 194, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "0V");
		drawText(vg, 37.5, 210, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-1V");
		drawText(vg, 37.5, 226, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-2V");
		drawText(vg, 37.5, 242, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-3V");
		drawText(vg, 37.5, 258, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-4V");
		drawText(vg, 37.5, 274, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-5V");
		drawText(vg, 37.5, 290, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-6V");
		drawText(vg, 37.5, 306, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-7V");
		drawText(vg, 37.5, 322, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-8V");
		drawText(vg, 37.5, 338, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-9V");
		drawText(vg, 37.5, 354, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE, 8, gScheme.getContrast(module), "-10");
	}
};

struct SS_220 : Module {
	static constexpr int deviceCount = 12;
	static constexpr int deviceSetCount = 10;
	SS_220() : Module() {
		config(0, 0, deviceCount * deviceSetCount, 0);
	}
	void process(const ProcessArgs &args) override {
		for (int j = 0; j < deviceSetCount; j++) {
			for (int i = 0; i < deviceCount; i++) {
				outputs[j * deviceCount + i].setVoltage((j - 5.0f) + 1.0f * i / 12.0f);
			}
		}
	}
};

struct SS220 : SchemeModuleWidget {
	SS220(SS_220 *module) : SchemeModuleWidget(module) {
		this->box.size = Vec(300, 380);
		addChild(new SchemePanel(this->box.size));
		for (int j = 0; j < SS_220::deviceSetCount; j++) {
			for (int i = 0; i < SS_220::deviceCount; i++) {
				addOutput(createOutputCentered<SilverPort>(Vec(15 + 30 * j, 31.5 + i * 29), module, j * SS_220::deviceCount + i));
			}
		}
	}
	void render(NVGcontext *vg, SchemeCanvasWidget *canvas) override {
		drawBase(vg, "SS-220");
		nvgFillColor(vg, gScheme.getAlternative(module));
		nvgStrokeColor(vg, gScheme.getContrast(module));
		nvgStrokeWidth(vg, 1);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 17, 296, 28, 2);
		nvgRoundedRect(vg, 2, 75, 296, 28, 2);
		nvgRoundedRect(vg, 2, 133, 296, 28, 2);
		nvgRoundedRect(vg, 2, 162, 296, 28, 2);
		nvgRoundedRect(vg, 2, 220, 296, 28, 2);
		nvgRoundedRect(vg, 2, 278, 296, 28, 2);
		nvgRoundedRect(vg, 2, 336, 296, 28, 2);
		nvgFill(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 150.5, 14.5, 29, 352, 2);
		nvgStroke(vg);
	}
};

Model *modelSS112 = createModel<SS_112, SS112>("SS-112");
Model *modelSS208 = createModel<SS_208, SS208>("SS-208");
Model *modelSS212 = createModel<SS_212, SS212>("SS-212");
Model *modelSS220 = createModel<SS_220, SS220>("SS-220");
Model *modelSS221 = createModel<SS_221, SS221>("SS-221");
