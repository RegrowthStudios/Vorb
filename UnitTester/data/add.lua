function add (x, y)
  x,y,z = getUnitX()
  print(x)
  print(y)
  print(z)
  printVec(z,y,x)

  toValue = TestObject_increment()
  print(toValue)
  valEnum(1)
  valPtr(nil)
  valRef(nil)
  print("Adding X+Y+TO.value\n")
  return x + y + toValue
end

function registerFuncs (env)
  register(env, "add", "add")
end