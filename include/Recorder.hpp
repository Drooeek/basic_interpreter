#pragma once

#include <map>
#include <memory>
#include <vector>
#include <optional>

#include "Statement.hpp"

class Recorder {
 public:
  ~Recorder();

  void add(int line, std::unique_ptr<Statement> stmt);
  void remove(int line);
  const Statement* get(int line) const noexcept;
  bool hasLine(int line) const noexcept;
  void clear() noexcept;
  void printLines() const;
  std::optional<int> nextLine(int line) const noexcept;
  std::optional<int> firstLine() const noexcept;

 private:
  std::map<int, std::unique_ptr<Statement>> stmts_;
};
