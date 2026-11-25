// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include "Statement.hpp"

#include "utils/Error.hpp"

Program::Program() { resetAfterRun(); }

void Program::addStmt(int line, std::unique_ptr<Statement> stmt) {
    recorder_.add(line, std::move(stmt));
}
void Program::removeStmt(int line) {
    recorder_.remove(line);
}

void Program::run() {
    resetAfterRun();
    std::optional<int> firstLine = recorder_.firstLine();
    if (!firstLine.has_value()) { programEnd(); return; }

    programCounter_ = firstLine.value();
    while (!programEnd_) {
      if (!recorder_.hasLine(programCounter_)) {
        throw BasicError("LINE NUMBER ERROR");
      }

      int currentLine = programCounter_;
      const Statement* stmt = recorder_.get(programCounter_);

      execute(stmt);
      if (programEnd_) return;

      if (programCounter_ == currentLine) {
        std::optional<int> nextLine = recorder_.nextLine(currentLine);

        if (!nextLine.has_value()) { programEnd(); return; }

        programCounter_ = nextLine.value();
      } 
    }
}
void Program::list() const { recorder_.printLines(); }
void Program::clear() { recorder_.clear(); vars_.clear(); resetAfterRun(); }

void Program::execute(const Statement* stmt) { if (stmt) stmt->execute(vars_, *this); }

int Program::getPC() const noexcept { return programCounter_; }
void Program::changePC(int line) { programCounter_ = line; }
void Program::programEnd() { programEnd_ = true; }

void Program::resetAfterRun() noexcept { 
  programCounter_ = -1;
  programEnd_ = false; 
}