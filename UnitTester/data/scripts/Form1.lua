function init()
  b1 = Form.makeButton("Button1", 40, 40, 400, 400)
  Button.setTextColor(b1, 255, 0, 0, 255)
  Button.setText(b1, "I made this with LUA!")
end

Vorb.register("init", init)