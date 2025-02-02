/**************************************************************
*
* The graphical styles and images encoded in this source file 
* are copyright © 2018 David O'Rourke
*
**************************************************************/

#include "../SubmarineFree.hpp"
#include "color.hpp"
	void LightSlider::draw(const DrawArgs &args) {
		float offset = rescale(paramQuantity->getValue(), paramQuantity->getMinValue(), paramQuantity->getMaxValue(), 0, box.size.y - 12);
		NVGcolor lcol = enabled?color:nvgRGB(0x4a,0x4a,0x4a);
		
		nvgFillColor(args.vg, nvgRGB(0, 0, 0));	
		nvgStrokeColor(args.vg, nvgRGB(0xff, 0xff, 0xff));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgRect(args.vg, box.size.x / 2 - 2, 4, 4, box.size.y - 8);
		nvgFill(args.vg);
		nvgBeginPath(args.vg);
		nvgRect(args.vg, box.size.x / 2 - 13, offset, 26, 12);
		nvgFill(args.vg);
	// Light
	{
		nvgBeginPath(args.vg);
		nvgRect(args.vg, box.size.x / 2 - 13, offset + 4, 26, 4);
		if (gScheme.isFlat) {
			nvgFillColor(args.vg, lcol);
		}
		else {
			NVGpaint paint;
			NVGcolor ocol = color::mult(lcol, 0.1);
			paint = nvgRadialGradient(args.vg, box.size.x / 2, offset + 6, 2, 8, lcol, ocol);
			nvgFillPaint(args.vg, paint);
		}
		nvgFill(args.vg);
	}
	
	// Halo
	if (!gScheme.isFlat) {
		nvgBeginPath(args.vg);
		nvgRect(args.vg, box.size.x / 2 - 20, offset - 4, 40, 20);
		NVGpaint paint;
		NVGcolor icol = color::mult(lcol, 0.08);
		NVGcolor ocol = nvgRGB(0, 0, 0);
		paint = nvgRadialGradient(args.vg, box.size.x / 2, offset + 6, 4, 20, icol, ocol);
		nvgFillPaint(args.vg, paint);
		nvgGlobalCompositeOperation(args.vg, NVG_LIGHTER);
		nvgFill(args.vg);	
	}
	}

	void LightSlider::onDragMove(const event::DragMove &e) {
		event::DragMove e2 = e;
		e2.mouseDelta = Vec(e.mouseDelta.x, -e.mouseDelta.y);
		LightKnob::onDragMove(e2);
	}
