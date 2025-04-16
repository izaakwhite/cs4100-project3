#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "tree_node.h"

// Forward declarations
class Expression;
class Statement;

// Context for variable storage and tree node references
class Context {
public:
    std::map<std::string, int> variables;
    std::map<std::string, TreeNode*> nodes;
    
    TreeNode* findNode(const std::string& name) {
        auto it = nodes.find(name);
        if (it != nodes.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    void addNode(const std::string& name, TreeNode* node) {
        nodes[name] = node;
    }
};

// Base class for all expressions
class Expression {
public:
    virtual ~Expression() {}
    virtual std::string evaluateString(Context& context) {
        return std::to_string(evaluateInt(context));
    }
    virtual int evaluateInt(Context& context) {
        return 0;
    }
};

// Integer literal
class IntLiteral : public Expression {
private:
    int value;
public:
    IntLiteral(int val) : value(val) {}
    
    virtual int evaluateInt(Context& context) override {
        return value;
    }
};

// String literal
class StringLiteral : public Expression {
private:
    std::string value;
public:
    StringLiteral(const std::string& val) : value(val) {
        // Remove quotes from string literals
        if (value.size() >= 2 && value[0] == '"' && value[value.size()-1] == '"') {
            value = value.substr(1, value.size()-2);
        }
    }
    
    virtual std::string evaluateString(Context& context) override {
        return value;
    }
    
    virtual int evaluateInt(Context& context) override {
        try {
            return std::stoi(value);
        } catch(...) {
            return 0;
        }
    }
};

// Variable reference
class Variable : public Expression {
private:
    std::string name;
public:
    Variable(const std::string& n) : name(n) {}
    
    virtual int evaluateInt(Context& context) override {
        auto it = context.variables.find(name);
        return it != context.variables.end() ? it->second : 0;
    }
    
    virtual std::string evaluateString(Context& context) override {
        return std::to_string(evaluateInt(context));
    }
};

// Binary operations for expressions
class BinaryExpr : public Expression {
protected:
    Expression* left;
    Expression* right;
public:
    BinaryExpr(Expression* l, Expression* r) : left(l), right(r) {}
    virtual ~BinaryExpr() {
        delete left;
        delete right;
    }
};

// Addition operation
class AddExpr : public BinaryExpr {
public:
    AddExpr(Expression* l, Expression* r) : BinaryExpr(l, r) {}
    
    virtual int evaluateInt(Context& context) override {
        return left->evaluateInt(context) + right->evaluateInt(context);
    }
    
    virtual std::string evaluateString(Context& context) override {
        return left->evaluateString(context) + right->evaluateString(context);
    }
};

// Base class for all statements
class Statement {
public:
    virtual ~Statement() {}
    virtual void execute(Context& context) = 0;
};

// Compound statement (list of statements)
class CompoundStatement : public Statement {
private:
    std::vector<Statement*> statements;
public:
    CompoundStatement() {}
    ~CompoundStatement() {
        for (auto stmt : statements) {
            delete stmt;
        }
    }
    
    void addStatement(Statement* stmt) {
        statements.push_back(stmt);
    }
    
    virtual void execute(Context& context) override {
        for (auto stmt : statements) {
            stmt->execute(context);
        }
    }
};

// BuildNode statement
class BuildNodeStatement : public Statement {
private:
    Expression* nameExpr;
    Expression* weightExpr;
    Expression* parentExpr;
    bool hasParent;
    
public:
    BuildNodeStatement(Expression* name, Expression* weight, Expression* parent = nullptr) 
        : nameExpr(name), weightExpr(weight), parentExpr(parent), hasParent(parent != nullptr) {}
    
    ~BuildNodeStatement() {
        delete nameExpr;
        delete weightExpr;
        if (hasParent) delete parentExpr;
    }
    
    virtual void execute(Context& context) override {
        std::string nodeName = nameExpr->evaluateString(context);
        int nodeWeight = weightExpr->evaluateInt(context);
        
        TreeNode* node = new TreeNode(nodeName, nodeWeight);
        // We could store the weight in the node if needed
        
        context.addNode(nodeName, node);
        
        if (hasParent) {
            std::string parentName = parentExpr->evaluateString(context);
            TreeNode* parent = context.findNode(parentName);
            if (parent) {
                parent->addChild(node);
            } else {
                std::cerr << "Parent node '" << parentName << "' not found!" << std::endl;
                delete node;
                context.nodes.erase(nodeName);
            }
        }
    }
};

// For loop statement
class ForStatement : public Statement {
private:
    std::string varName;
    Expression* startExpr;
    Expression* endExpr;
    CompoundStatement* body;
    
public:
    ForStatement(const std::string& var, Expression* start, Expression* end, CompoundStatement* b)
        : varName(var), startExpr(start), endExpr(end), body(b) {}
    
    ~ForStatement() {
        delete startExpr;
        delete endExpr;
        delete body;
    }
    
    virtual void execute(Context& context) override {
        int start = startExpr->evaluateInt(context);
        int end = endExpr->evaluateInt(context);
        
        for (int i = start; i <= end; i++) {
            context.variables[varName] = i;
            body->execute(context);
        }
    }
};

// Program - top level container
class Program {
private:
    CompoundStatement* statements;
    Context context;              // one shared context
    
public:
    Program(CompoundStatement* stmts) : statements(stmts) {}
    
    ~Program() {
        delete statements;
    }
    
    void execute() {
        statements->execute(context);
    }
    
    TreeNode* getRoot() {
        return context.findNode("root");
    }
};

#endif // PARSE_TREE_H