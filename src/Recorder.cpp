// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"

#include <iostream>

Recorder::~Recorder() = default;

void Recorder::add(int line, std::unique_ptr<Statement> stmt) {
  stmts_[line] = std::move(stmt);
}
void Recorder::remove(int line) { stmts_.erase(line); }

const Statement* Recorder::get(int line) const noexcept { 
  auto it = stmts_.find(line);
  if (it == stmts_.end()) return nullptr;
  return it->second.get();
}
bool Recorder::hasLine(int line) const noexcept { return stmts_.count(line); }

void Recorder::clear() noexcept { stmts_.clear(); }

void Recorder::printLines() const {
  for (auto it = stmts_.begin(); it != stmts_.end(); it++)
    std::cout << it->second->text() << '\n';
}
std::optional<int> Recorder::nextLine(int line) const noexcept {
  auto it = stmts_.find(line);
  if (it == stmts_.end()) return std::nullopt;
  ++it;
  if (it == stmts_.end()) return std::nullopt;
  return it->first;
}
std::optional<int> Recorder::firstLine() const noexcept {
  if (stmts_.empty()) return std::nullopt;
  return stmts_.begin()->first;
}