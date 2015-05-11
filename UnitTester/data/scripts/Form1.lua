function init()
  tex1 = loadTextureDefault("data/button_test.jpg")
  tex2 = loadTextureDefault("data/checked_test.jpg")
  -- Test Button
  b1 = Form.makeButton("Button1", 40, 40, 200, 200)
  Button.setTextColor(b1, 255, 0, 0, 255)
  Button.setText(b1, "I made this with LUA!")
  Button.setTextAlign(b1, TextAlign.CENTER)
  Button.setTexture(b1, tex1)
  -- Test Slider
  s1 = Form.makeSlider("Slider1", 450, 80, 200, 15)
  -- Test Checkbox
  c1 = Form.makeCheckBox("CheckBox1", 60, 300, 30, 30)
  CheckBox.setText(c1, "Yay Checkbox")
  CheckBox.setTextColor(c1, 255, 0, 0, 255)
  CheckBox.setCheckedTexture(c1, tex2)
end

Vorb.register("init", init)