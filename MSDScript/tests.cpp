//
//  tests.cpp
//  HW2_Expression
//
//  Created by Yue Sun on 1/12/23.
//

#include "catch.h"
#include "expr.hpp"
#include <iostream>

TEST_CASE("Equals") {
    SECTION("Num") {
        NumExpr num1(1);
        NumExpr num2(2);
        NumExpr num3(1);
        NumExpr num4(-1);
        CHECK_FALSE(num1.equals(&num2));
        CHECK_FALSE(num2.equals(&num1));
        CHECK(num1.equals(&num3));
        CHECK(num3.equals(&num1));
        CHECK_FALSE(num2.equals(&num3));
        CHECK_FALSE(num3.equals(&num2));
        CHECK(!num1.equals(&num4));
    }
    
    SECTION("Add_simple") {
        AddExpr add1(new NumExpr(1), new NumExpr(2));
        AddExpr add2(new NumExpr(2), new NumExpr(1));
        AddExpr add3(new NumExpr(1), new NumExpr(2));
        CHECK_FALSE(add1.equals(&add2));
        CHECK_FALSE(add2.equals(&add1));
        CHECK(add1.equals(&add3));
        CHECK(add3.equals(&add1));
        CHECK(!add2.equals(&add3));
        CHECK(!add3.equals(&add2));
    }
    
    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 2), new MultExpr(3, 4));
        AddExpr add2(new MultExpr(3, 4), new AddExpr(1, 2));
        AddExpr add3(new AddExpr(1, 2), new MultExpr(3, 4));
        CHECK_FALSE(add1.equals(&add2));
        CHECK_FALSE(add2.equals(&add1));
        CHECK(add1.equals(&add3));
        CHECK(add3.equals(&add1));
        CHECK(!add2.equals(&add3));
        CHECK(!add3.equals(&add2));
    }

    SECTION("Mult_simple") {
        MultExpr mult1(1, 2);
        MultExpr mult2(2, 1);
        MultExpr mult3(1, 2);
        CHECK_FALSE(mult1.equals(&mult2));
        CHECK_FALSE(mult2.equals(&mult1));
        CHECK(mult1.equals(&mult3));
        CHECK(mult3.equals(&mult1));
        CHECK(!mult2.equals(&mult3));
        CHECK(!mult3.equals(&mult2));
    }
    
    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(1, 2), new MultExpr(3, 4));
        MultExpr mult2(new MultExpr(3, 4), new AddExpr(1, 2));
        MultExpr mult3(new AddExpr(1, 2), new MultExpr(3, 4));
        CHECK_FALSE(mult1.equals(&mult2));
        CHECK_FALSE(mult2.equals(&mult1));
        CHECK(mult1.equals(&mult3));
        CHECK(mult3.equals(&mult1));
        CHECK(!mult2.equals(&mult3));
        CHECK(!mult3.equals(&mult2));
    }

    SECTION("Variable") {
        VarExpr var1("a");
        VarExpr var2("1");
        VarExpr var3("a");
        CHECK_FALSE(var1.equals(&var2));
        CHECK(var1.equals(&var3));
        CHECK(!var2.equals(&var3));
    }
};

TEST_CASE("Interp") {
    SECTION("Num") {
        CHECK((new NumExpr(0)) -> interp() == 0);
        CHECK((new NumExpr(-100))-> interp() == -100);
    }
    
    SECTION("Add_simple") {
        CHECK((new AddExpr(1, 3)) -> interp() == 4);
        CHECK((new AddExpr(-2019, 5072)) -> interp() == 3053);
        CHECK((new AddExpr(new NumExpr(57), new AddExpr(3, -800))) -> interp() == -740);
    }
    
    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 9), new MultExpr(2, 50));
        AddExpr add2(new AddExpr(-31000, 19), new AddExpr(20, 61));
        AddExpr add3(new AddExpr(1010, 2), new MultExpr(0, 10000));
        CHECK(add1.interp() == 110);
        CHECK(add2.interp() == -30900);
        CHECK(add3.interp() == 1012);
    }
    
    SECTION("Mult_simple") {
        MultExpr mult1(-3, 2);
        MultExpr mult2(-1034, -730);
        MultExpr mult3(12345678, 0);
        CHECK(mult1.interp() == -6);
        CHECK(mult2.interp() == 754820);
        CHECK(mult3.interp() == 0);
    }
    
    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(2, 3), new MultExpr(4, 5));
        MultExpr mult2(new MultExpr(-5, 10), new MultExpr(-300, 20));
        MultExpr mult3(new MultExpr(2, 6), new AddExpr(1, -9));
        CHECK(mult1.interp() == 100);
        CHECK(mult2.interp() == 300000);
        CHECK(mult3.interp() == -96);
    }
    
    SECTION("Variable") {
        CHECK_THROWS_WITH((new VarExpr("x")) -> interp(), "A variable has no value!");
        CHECK_THROWS_WITH((new AddExpr(6, "y")) -> interp(), "A variable has no value!");
    }
}

TEST_CASE("has_variable") {
    SECTION("Num") {
        CHECK(!(new NumExpr(3)) -> has_variable());
    }
    
    SECTION("Add_simple") {
        CHECK((new AddExpr(7, "x")) -> has_variable());
        CHECK(!(new AddExpr(2, 3)) -> has_variable());
    }
    
    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 5), new AddExpr(new NumExpr(1), new AddExpr(9, "speed")));
        AddExpr add2(new AddExpr(1010, 2), new MultExpr(0, 10000));
        CHECK(add1.has_variable());
        CHECK(!add2.has_variable());
    }
    
    SECTION("Mult_simple") {
        MultExpr mult1(3, 2);
        MultExpr mult2(8, "yy");
        CHECK(!mult1.has_variable());
        CHECK(mult2.has_variable());
    }
    
    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(2, 3), new MultExpr(4, 5));
        MultExpr mult2(new MultExpr(1, 5), new AddExpr(new NumExpr(1), new AddExpr(9, "scale")));
        CHECK(!mult1.has_variable());
        CHECK(mult2.has_variable());
    }
    
    SECTION("Variable") {
        CHECK((new VarExpr("any thing")) -> has_variable());
    }
}

TEST_CASE("subst") {
    SECTION("Num") {
        CHECK((new NumExpr(4)) -> subst("x", new NumExpr(3)) -> equals(new NumExpr(4)));
    }
    
    SECTION("Add_simple") {
        CHECK(AddExpr("x", 7).subst("x", new VarExpr("y"))->equals(new AddExpr("y", 7)));
        CHECK(AddExpr(2, 3).subst("x", new NumExpr(1))->equals(new AddExpr(2, 3)));
    }
    
    SECTION("Add_nested") {
        AddExpr add1(new AddExpr(1, 5), new AddExpr("a", 9));
        AddExpr res11(new AddExpr(1, 5), new AddExpr(100, 9));
        AddExpr res12(new AddExpr(1, 5), new AddExpr("b", 9));
        AddExpr res13(new AddExpr(1, 5), new AddExpr("a", 9));
        AddExpr add2(new AddExpr("x", 3), new MultExpr("y", -10));
        AddExpr res21(new AddExpr(new VarExpr("y"), new NumExpr(3)), new MultExpr("y", -10));
        AddExpr res22(new AddExpr(new AddExpr(1, 3), new NumExpr(3)), new MultExpr("y", -10));
        CHECK(add1.subst("a", new NumExpr(100)) -> equals(&res11));
        CHECK(add1.subst("a", new VarExpr("b")) -> equals(&res12));
        CHECK(add1.subst("b", new NumExpr(7)) -> equals(&res13));
        CHECK(add2.subst("x", new VarExpr("y")) -> equals(&res21));
        CHECK(add2.subst("x", new AddExpr(1, 3)) -> equals(&res22));
    }
    
    SECTION("Mult_simple") {
        CHECK( MultExpr("x", 7).subst("x", new VarExpr("y"))->equals(new MultExpr("y", 7)));
        CHECK(MultExpr(2, 3).subst("x", new NumExpr(1))->equals(new MultExpr(2, 3)));
    }
    
    SECTION("Mult_nested") {
        MultExpr mult1(new AddExpr(1, 5), new MultExpr("a", 9));
        MultExpr res11(new AddExpr(1, 5), new MultExpr(100, 9));
        MultExpr res12(new AddExpr(1, 5), new MultExpr("b", 9));
        MultExpr res13(new AddExpr(1, 5), new MultExpr("a", 9));
        MultExpr mult2(new MultExpr(new VarExpr("x"), new NumExpr(3)), new MultExpr("y", -10));
        MultExpr res21(new MultExpr(new VarExpr("y"), new NumExpr(3)), new MultExpr("y", -10));
        MultExpr res22(new MultExpr(new AddExpr(1, 3), new NumExpr(3)), new MultExpr("y", -10));
        CHECK(mult1.subst("a", new NumExpr(100)) -> equals(&res11));
        CHECK(mult1.subst("a", new VarExpr("b")) -> equals(&res12));
        CHECK(mult1.subst("b", new NumExpr(7)) -> equals(&res13));
        CHECK(mult2.subst("x", new VarExpr("y")) -> equals(&res21));
        CHECK(mult2.subst("x", new AddExpr(1, 3)) -> equals(&res22));
    }
    
    SECTION("Variable") {
        CHECK((new VarExpr("x"))->subst("x", new AddExpr("y", 7)) -> equals(new AddExpr("y", 7)));
        CHECK(VarExpr("x").subst("y", new NumExpr(3)) -> equals(new VarExpr("x")));
    }
}

TEST_CASE("Interpt + Subst") {
    AddExpr add1(new AddExpr(1, 5), new AddExpr("a", 9));
    CHECK(add1.subst("a", new NumExpr(100)) -> interp() == 115);
    MultExpr mult1(new AddExpr(1, 5), new AddExpr("a", 9));
    CHECK(mult1.subst("a", new NumExpr(100)) -> interp() == 654);
        
};

TEST_CASE("to_string + print") {
    SECTION("Num") {
        CHECK(NumExpr(10).to_string() == "10");
        CHECK(NumExpr(-10).to_string() == "-10");
    }
    
    SECTION("Add_simple") {
        CHECK(AddExpr(1, 2).to_string() == "(1+2)");
        CHECK(AddExpr("x", 2).to_string() == "(x+2)");
        CHECK(AddExpr(-3, "speed").to_string() == "(-3+speed)");
        CHECK(AddExpr(-10, -5).to_string() == "(-10+-5)");
    }
    
    SECTION("Add_nested") {
        AddExpr add1(new NumExpr(1), new AddExpr(-10, 2));
        CHECK(add1.to_string() == "(1+(-10+2))");
        AddExpr add2(new AddExpr(1, 2), new AddExpr(3, 4));
        CHECK(add2.to_string() == "((1+2)+(3+4))");
        AddExpr add3(new AddExpr(1001, 50), new VarExpr("p"));
        CHECK(add3.to_string() == "((1001+50)+p)");
    }
    
    SECTION("Mult_simple") {
        CHECK(MultExpr(-1, 2).to_string() == "(-1*2)");
        CHECK(MultExpr("x", "y").to_string() == "(x*y)");
        CHECK(MultExpr("z", 2).to_string() == "(z*2)");
    }
    
    SECTION("Mult_nested") {
        MultExpr mult1(new NumExpr(1), new MultExpr(-10, 2));
        CHECK(mult1.to_string() == "(1*(-10*2))");
        MultExpr mult2(new AddExpr(2, 7), new MultExpr(5, -20));
        CHECK(mult2.to_string() == "((2+7)*(5*-20))");
        MultExpr mult3(new MultExpr(1001, 50), new VarExpr("p"));
        CHECK(mult3.to_string() == "((1001*50)*p)");
    }
    
    SECTION("Variable") {
        CHECK(VarExpr("xyz").to_string() == "xyz");
    }
};

TEST_CASE("pretty_print") {
    SECTION("num") {
        CHECK(NumExpr(1).to_pretty_string() == "1");
    }
    
    SECTION("add") {
        AddExpr add1(new AddExpr(1, 2), new NumExpr(3));
        CHECK(add1.to_pretty_string() == "(1 + 2) + 3");
        AddExpr add2(new NumExpr(1), new AddExpr(2, 3));
        CHECK(add2.to_pretty_string() == "1 + 2 + 3");
        AddExpr add3(new AddExpr(1, 2), new AddExpr(new NumExpr(3), new AddExpr(4, 5)));
        CHECK(add3.to_pretty_string() == "(1 + 2) + 3 + 4 + 5");
        AddExpr add4(new AddExpr(1, 2), new AddExpr(new AddExpr(3, 4), new NumExpr(5)));
        CHECK(add4.to_pretty_string() == "(1 + 2) + (3 + 4) + 5");
    }
    
    SECTION("mult") {
        MultExpr mult1(new MultExpr(1, 2), new NumExpr(3));
        CHECK(mult1.to_pretty_string() == "(1 * 2) * 3");
        MultExpr mult2(new NumExpr(1), new MultExpr(2, 3));
        CHECK(mult2.to_pretty_string() == "1 * 2 * 3");
        MultExpr mult3(new MultExpr(1, 2), new MultExpr(new NumExpr(3), new MultExpr(4, 5)));
        CHECK(mult3.to_pretty_string() == "(1 * 2) * 3 * 4 * 5");
        MultExpr mult4(new MultExpr(1, 2), new MultExpr(new MultExpr(3, 4), new NumExpr(5)));
        CHECK(mult4.to_pretty_string() == "(1 * 2) * (3 * 4) * 5");
    }
    
    SECTION("add + mult") {
        CHECK(AddExpr(new NumExpr(1), new MultExpr(2, 3)).to_pretty_string() == "1 + 2 * 3");
        CHECK(MultExpr(new NumExpr(1), new AddExpr(2, 3)).to_pretty_string() == "1 * (2 + 3)");
    }
    
    SECTION("Variable") {
        CHECK(VarExpr("xyz").to_string() == "xyz");
    }
    
}