varString = "ConfigString"
varInteger = 123

-- Lua中模拟一个类

table = {name = "Mike", age = 20, x = 0, y = 0}
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

object1 = table:new(100, 20)
object2 = table:new(90, 30)
print("name = " .. object1.name)
object1:sub()
object1:test()
print("age = " .. object1.age)
object1:sub()
object1:test()

varString = object1.name
varInteger = object1.x + object2.y

-- make a complex table for c++ reading

resultTable = {
	name = "Merciel",
	company = "GAMTEQ",
	age = 1,
	skill = {"C++", "Lua", "JavaScript", db = {dbname = "Oracle", dbversion = "9.1"}},
	objType = {type1 = "Master", type2 = "Monster"},
	eState = {0, 1, 2, {8, 9, 10}, 3}
}























