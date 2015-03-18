function add (x, y)
  TestObject_increment()
  print("Adding X+Y\n");
  return x + y
end

function registerFuncs (env)
  register(env, "add", "add")
end