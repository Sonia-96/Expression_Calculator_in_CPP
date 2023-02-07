//
//  expr.hpp
//  HW2_Expression
//
//  Created by Yue Sun on 1/12/23.
//

#ifndef expr_hpp
#define expr_hpp

#include <sstream>
#include <string>

enum precedence_t {
    prec_none, // 0
    prec_add, // 1
    prec_mult, // 2
};

/** \brief
 * Expr class - the base class for all expressions
 */
class Expr {
public:
    precedence_t prec; // !< the precedence of an expression
    virtual bool equals(Expr* expr)=0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string s, Expr* expr) = 0;
    virtual void print(std::ostream& out) = 0;
    virtual void pretty_print(std::ostream& out) = 0;
    std::string to_string();
    precedence_t get_precedence() {
        return prec;
    }
    std::string to_pretty_string();
};

/** \brief
 * NumExpr class - for number expressions
 */
class NumExpr : public Expr {
private:
    int val;
public:
    NumExpr(int v);
    bool equals(Expr* expr);
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* expr);
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
};

/** \brief
 * AddExpr class - for add expressions
 */
class AddExpr : public Expr {
private:
    Expr* lhs;
    Expr* rhs;
public:
    AddExpr(Expr *left, Expr *right);
    AddExpr(int left, int right);
    AddExpr(std::string left, int right);
    AddExpr(int left, std::string right);
    AddExpr(std::string left, std::string right);
    bool equals(Expr* expr);
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* expr);
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
};

/** \brief
 * MultExpr class - for multiplication expressions
 */
class MultExpr : public Expr {
public:
    Expr* lhs;
    Expr* rhs;
public:
    MultExpr(Expr *left, Expr *right);
    MultExpr(int left, int right);
    MultExpr(std::string left, int right);
    MultExpr(int left, std::string right);
    MultExpr(std::string left, std::string right);
    bool equals(Expr* expr);
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* expr);
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
};

/** \brief
 * VarExpr class - for variable expressions
 */
class VarExpr : public Expr {
private:
    std::string val;
public:
    VarExpr(std::string s);
    bool equals(Expr* expr);
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* expr);
    void print(std::ostream& out);
    void pretty_print(std::ostream& out);
};

#endif /* expr_hpp */
