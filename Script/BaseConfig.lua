varString = "ConfigString"
varInteger = 123

-- define class table

table = {name = "John", age = 20, x = 0, y = 0}
table.__index = table;

function table:new(x, y)
	local self = {}
	setmetatable(self, table)
	self.x = x
	self.y = y
	return self
end

function table:test()
	print(self.x, self.y)
end

function table:sub()
	self.x = self.x - 1
	self.y = self.y - 1
end

-- try table

object1 = table:new(10, 2)
object2 = table:new(9, 3)
print("name = " .. object1.name)
object1:sub()
object1:test()
print("age = " .. object1.age)
object1:sub()
object1:test()

varString = object1.name
varInteger = object1.x + object2.y



