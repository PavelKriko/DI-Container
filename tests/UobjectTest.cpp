#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include"IoC/IoC.h"
#include"IoC/Strategies/BuildTank.h"
#include"UObject/UObject.h"
#include"vectors/vectorOps.h"

TEST(Uobject, ObjectsMethod)
{ 
    UObject obj;
    obj.addProperty("position", std::vector<int>{1, 1});
    const std::vector<int> pos{ 1,1 };
    EXPECT_EQ(std::any_cast<std::vector<int>>(obj.getProperty("position")), pos);                  //наличие свойства
    EXPECT_THROW(obj.getProperty("HP"), std::out_of_range);                                        //ошибка, свойства нет
    EXPECT_THROW(obj.addProperty("position", std::vector<int>{2, 2}), std::runtime_error);         //ошибка, свойство уже есть
    obj.setPropertyValue("position", std::vector<int>{3, 3});
    const std::vector<int> new_pos{ 3,3 };
    EXPECT_EQ(std::any_cast<std::vector<int>>(obj.getProperty("position")), new_pos);              //устанавливается новое значение свойства
    EXPECT_THROW(obj.setPropertyValue("HP", int{ 100 }), std::out_of_range);                       //ошибка при установке нового значения несуществующего свойства
    EXPECT_THROW(obj.eraseProperty("HP"), std::runtime_error);                                     //ошибка при удалении несуществующего свойства
    obj.eraseProperty("position");
    EXPECT_THROW(std::any_cast<std::vector<int>>(obj.getProperty("position")), std::out_of_range); //ошибка, свойства больше нет 

}
