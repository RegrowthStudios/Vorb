function add (x, y)
  cSum(myData, x, y)
  print("Adding X+Y\n");
  return x + y
end

function registerFuncs (env)
  register(env, "add", "add")
end