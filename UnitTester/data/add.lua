function add (x, y)
  toValue = TestObject_increment()
  print(toValue)
  Test.Namespace.valEnum(1)
  Test.Namespace.valPtr(nil)
  Test.Namespace.valRef(nil)
  v = stuffs(1,2,3)
  print("Adding X+Y+TO.value\n")
  return x + y + toValue
end

Vorb.register("add", add)
Vorb.register("add2", function (x, y)
  return x + y
end)
