function init()
  tex1 = loadTextureDefault("data/button_test.jpg")
  b1 = Form.makeButton("Button1", 40, 40, 200, 200)
  Button.setTextColor(b1, 255, 0, 0, 255)
  Button.setText(b1, "I made this with LUA!")
  Button.setTextAlign(b1, TextAlign.CENTER)
  Button.setTexture(b1, tex1)
end

Vorb.register("init", init)