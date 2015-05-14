function init()
  tex1 = loadTextureDefault("data/button_test.jpg")
  tex2 = loadTextureDefault("data/checked_test.jpg")
  -- Test Button
  b1 = Form.makeButton(this, "Button1", 40, 40, 100, 100)
  Button.setTextColor(b1, 255, 0, 0, 255)
  Button.setBackColor(b1, 155, 155, 155, 255);
  Button.setText(b1, "Left dock")
  Button.setTextAlign(b1, TextAlign.CENTER)
  Button.setDock(b1, DockStyle.LEFT)
  
  b2 = Form.makeButton(this, "Button2", 40, 40, 100, 100)
  Button.setTextColor(b2, 255, 0, 0, 255)
  Button.setBackColor(b2, 100, 100, 100, 255);
  Button.setText(b2, "Top dock")
  Button.setTextAlign(b2, TextAlign.CENTER)
  Button.setDock(b2, DockStyle.TOP)
  
  b3 = Form.makeButton(this, "Button3", 40, 40, 100, 100)
  Button.setTextColor(b3, 255, 0, 0, 255)
  Button.setBackColor(b3, 68, 68, 68, 255);
  Button.setText(b3, "Right dock")
  Button.setTextAlign(b3, TextAlign.CENTER)
  Button.setDock(b3, DockStyle.RIGHT)
  b6 = Form.makeButton("Button3", 40, 40, 100, 100)
  Button.setTextColor(b6, 255, 0, 0, 255)
  Button.setBackColor(b6, 68, 68, 68, 255);
  Button.setText(b6, "Right dock")
  Button.setTextAlign(b6, TextAlign.CENTER)
  Button.setDock(b6, DockStyle.RIGHT)
  
  b4 = Form.makeButton(this, "Button4", 40, 40, 100, 100)
  Button.setTextColor(b4, 255, 0, 0, 255)
  Button.setBackColor(b3, 79, 79, 79, 255);
  Button.setText(b4, "Bottom dock")
  Button.setTextAlign(b4, TextAlign.CENTER)
  Button.setDock(b4, DockStyle.BOTTOM)
  
  b5 = Form.makeButton(this, "Button5", 40, 40, 100, 100)
  Button.setTextColor(b5, 255, 0, 0, 255)
  Button.setText(b5, "Fill dock")
  Button.setTextAlign(b5, TextAlign.CENTER)
  Button.setDock(b5, DockStyle.FILL)
  -- Test Slider
 -- s1 = Form.makeSlider("Slider1", 450, 80, 200, 15)
  -- Test Checkbox
 -- c1 = Form.makeCheckBox("CheckBox1", 60, 300, 30, 30)
 -- CheckBox.setText(c1, "Yay Checkbox")
 -- CheckBox.setTextColor(c1, 255, 0, 0, 255)
 -- CheckBox.setCheckedTexture(c1, tex2)
  -- Test ComboBox
 -- cb1 = Form.makeComboBox("ComboBox1", 450, 200, 200, 30);
 -- ComboBox.setTextColor(cb1, 255, 0, 0, 255)
 -- ComboBox.addItem(cb1, "TestItem0")
 -- ComboBox.addItem(cb1, "TestItem1")
 -- ComboBox.addItem(cb1, "TestItem2")
 -- ComboBox.addItem(cb1, "TestItem3")
 -- ComboBox.addItem(cb1, "TestItem4")
 -- ComboBox.addItem(cb1, "TestItem5")
 -- ComboBox.selectItemAtIndex(cb1, 3)
end

Vorb.register("init", init)