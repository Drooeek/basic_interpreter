#pragma once

#include <memory>
#include <vector>

#include "Recorder.hpp"
#include "VarState.hpp"

class Statement;

class Program {
 public:
  Program();

  void addStmt(int line, std::unique_ptr<Statement> stmt);
  void removeStmt(int line);

  void run();
  void list() const;
  void clear();

  void execute(const Statement* stmt);

  int getPC() const noexcept;
  void changePC(int line);
  void programEnd();

  void initVars();
  void addVars();
  void removeVars();

 private:
  Recorder recorder_;
  std::vector<VarState> vars_;
  int programCounter_;
  bool programEnd_;

  void resetAfterRun() noexcept;
};
