#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.

// LET <varName> = <expr>
LetStatement::LetStatement(std::string source, std::string varName, std::unique_ptr<Expression> expr)
  : Statement(std::move(source)), varName_(std::move(varName)), expr_(std::move(expr)) {}

void LetStatement::execute(VarState& state, Program& program) const {
  state.setValue(varName_, expr_->evaluate(state));
}

// PRINT <expr>
PrintStatement::PrintStatement(std::string source, std::unique_ptr<Expression> expr)
  : Statement(std::move(source)), expr_(std::move(expr)) {}

void PrintStatement::execute(VarState& state, Program& program) const {
  std::cout << expr_->evaluate(state) << '\n';
}

// INPUT <varName>
InputStatement::InputStatement(std::string source, std::string varName)
  : Statement(std::move(source)), varName_(std::move(varName)) {}

void InputStatement::execute(VarState& state, Program& program) const {
  int value;
  std::string line;
  while (true) {
    std::cout << " ? ";
    if (!std::getline(std::cin, line)) {
      throw BasicError("INVALID NUMBER");
    }

    std::istringstream iss(line);
    if (iss >> value && iss.eof()) {
      state.setValue(varName_, value);
      break;
    } else {
      std::cout << "INVALID NUMBER\n";
    }
  }
}

// GOTO <targetLine>
GotoStatement::GotoStatement(std::string source, int targetLine)
  : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStatement::execute(VarState& state, Program& program) const {
  program.changePC(targetLine_);
}

// IF <leftExpr> <op> <rightExpr> THEN <targetLine>
IfStatement::IfStatement(std::string source, std::unique_ptr<Expression> leftExpr, char op, std::unique_ptr<Expression> rightExpr, int targetLine)
  : Statement(std::move(source)), leftExpr_(std::move(leftExpr)), op_(op), rightExpr_(std::move(rightExpr)), targetLine_(targetLine) {}

void IfStatement::execute(VarState& state, Program& program) const {
  int lhs = leftExpr_->evaluate(state);
  int rhs = rightExpr_->evaluate(state);
  bool isTrue = false;
  switch(op_) {
    case '=' : { isTrue = (lhs == rhs); break; }
    case '<' : { isTrue = (lhs < rhs); break; }
    case '>' : { isTrue = (lhs > rhs); break; }
  }
  if (isTrue) program.changePC(targetLine_);
}

// REM <comment>
RemStatement::RemStatement(std::string source, std::string comment)
  : Statement(std::move(source)), comment_(std::move(comment)) {}

void RemStatement::execute(VarState& state, Program& program) const {
  // DO NOTHING~
}

// END
EndStatement::EndStatement(std::string source)
  : Statement(std::move(source)) {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}

// INDENT
IndentStatement::IndentStatement(std::string source)
  : Statement(std::move(source)) {}

void IndentStatement::execute(VarState& state, Program& program) const {
  program.addVars();
}

// DEDENT
DedentStatement::DedentStatement(std::string source)
  : Statement(std::move(source)) {}

void DedentStatement::execute(VarState& state, Program& program) const {
  program.removeVars();
}
