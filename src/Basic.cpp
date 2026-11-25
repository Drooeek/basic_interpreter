#include <iostream>
#include <memory>
#include <string>
#include <optional>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      TokenStream tokens = lexer.tokenize(line);
      if (tokens.empty()) { continue; }

      const Token* firstToken = tokens.peek();

      // 解释器指令
      switch (firstToken->type) {
        case TokenType::RUN : {
          program.run();
          continue;
        } case TokenType::LIST : {
          program.list();
          continue;
        } case TokenType::CLEAR : {
          program.clear();
          continue;
        } case TokenType::QUIT : {
          return 0;
        } case TokenType::HELP : {
          std::cout << "Yeah~\n";
          continue;
        }
      }
      
      ParsedLine parsedLine = parser.parseLine(tokens, line);
      std::optional<int> lineOpt = parsedLine.getLine();

      // 一般指令（带行号）
      if (lineOpt.has_value()) {
        int lineNumber = lineOpt.value();
        Statement* stmtOpt = parsedLine.getStatement();
        if (stmtOpt == nullptr) {
          // 空行->删除该行
          program.removeStmt(lineNumber);
        } else {
          // 将 statement 所有权转交给 program->recorder
          std::unique_ptr<Statement> stmt(parsedLine.fetchStatement());
          program.addStmt(lineNumber, std::move(stmt));
        }
      }

      // 立即执行指令
      else {
        Statement* stmtOpt = parsedLine.getStatement();
        if (stmtOpt == nullptr) {
          throw BasicError("SYNTAX ERROR");
        }
        std::unique_ptr<Statement> stmt(parsedLine.fetchStatement());
        program.execute(stmt.get());
        // 离开作用域后，指针自动释放
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}