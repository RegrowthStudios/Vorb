function init()
b1 = Form.makeButton(this, "Button1", 40, 40, 100, 100)
Button.setTextColor(b1, 255, 0, 0, 255)
Button.setBackColor(b1, 155, 155, 155, 255);
Button.setText(b1, "Left dock")
Button.setTextAlign(b1, TextAlign.CENTER)
Button.setDock(b1, DockStyle.LEFT) 

l1 = Form.makeLabel(this, "Label1", 100, 100, 100, 16)
Label.setTextColor(l1, 255, 255, 255, 255)
Label.setText(l1, "TestLabel")
Label.setTextAlign(l1, TextAlign.CENTER)
end

Vorb.register("init", init)