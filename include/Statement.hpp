#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.

// LET <varName> = <expr>
class LetStatement : public Statement {
  public:
    explicit LetStatement(std::string source, std::string varName, std::unique_ptr<Expression> expr);
    void execute(VarState& state, Program& program) const override;

  private:
    std::string varName_;
    std::unique_ptr<Expression> expr_;
};

// PRINT <expr>
class PrintStatement : public Statement {
  public:
    explicit PrintStatement(std::string source, std::unique_ptr<Expression> expr);
    void execute(VarState& state, Program& program) const override;

  private:
    std::unique_ptr<Expression> expr_;
};

// INPUT <varName>
class InputStatement : public Statement {
  public:
    explicit InputStatement(std::string source, std::string varName);
    void execute(VarState& state, Program& program) const override;

  private:
    std::string varName_;
};

// GOTO <targetLine>
class GotoStatement : public Statement {
  public:
    explicit GotoStatement(std::string source, int targetLine);
    void execute(VarState& state, Program& program) const override;

  private:
    int targetLine_;
};

// IF <leftExpr> <op> <rightExpr> THEN <targetLine>
class IfStatement : public Statement {
  public:
    explicit IfStatement(std::string source, std::unique_ptr<Expression> leftExpr, char op, std::unique_ptr<Expression> rightExpr, int targetLine);
    void execute(VarState& state, Program& program) const override;

  private:
    std::unique_ptr<Expression> leftExpr_;
    char op_;
    std::unique_ptr<Expression> rightExpr_;
    int targetLine_;
};

// REM <comment>
class RemStatement : public Statement {
  public:
    explicit RemStatement(std::string source, std::string comment);
    void execute(VarState& state, Program& program) const override;

  private:
    std::string comment_;
};

// END
class EndStatement : public Statement {
  public:
    explicit EndStatement(std::string source);
    void execute(VarState& state, Program& program) const override;

  private:
};

// INDENT
class IndentStatement : public Statement {
  public:
    explicit IndentStatement(std::string source);
    void execute(VarState& state, Program& program) const override;

  private:
};

// DEDENT
class DedentStatement : public Statement {
  public:
    explicit DedentStatement(std::string source);
    void execute(VarState& state, Program& program) const override;

  private:
};