function init()
  tex1 = loadTextureDefault("data/button_test.jpg")
  -- Test Button
  b1 = Form.makeButton("Button1", 40, 40, 200, 200)
  Button.setTextColor(b1, 255, 0, 0, 255)
  Button.setText(b1, "I made this with LUA!")
  Button.setTextAlign(b1, TextAlign.CENTER)
  Button.setTexture(b1, tex1)
  -- Test Slider
  s1 = Form.makeSlider("Slider1", 450, 80, 200, 15);
end

Vorb.register("init", init)