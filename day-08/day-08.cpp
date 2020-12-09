#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Vm {
  // accumulator
  int acc = 0;

  // for each instruction, the number of times it has been hit
  unordered_map<int, int> hit_counter;

  // all the instructions
  vector<string> instructions;

  // program counter: index into the instructions vector
  int pc = 0;

  stack<int> jmps;
  stack<int> nops;
};

// runs the vm, returns true for a normal exit, false for inf loop
bool vm_run(Vm &vm, bool track = false) {
  // run the vm loop
  while (true) {
    // done with all instructions
    if (vm.pc >= vm.instructions.size()) return true;

    // incr hit count for this instruction
    vm.hit_counter[vm.pc]++;

    // hit something twice.
    if (vm.hit_counter[vm.pc] > 1) return false;

    // Instruction parsing
    string instr = vm.instructions[vm.pc];

    // nop, acc, or jmp
    string op = instr.substr(0, 3);

    // either 1 for +, or -1 for -
    int sign = instr[4] == '+' ? 1 : -1;
    int operand = stoi(instr.substr(5));
    int signed_operand = sign * operand;

    // in addition to the usual vm business, keep track of nops and jmps
    if (op == "nop") {
      if (track) vm.nops.push(vm.pc);

      vm.pc++;
    } else if (op == "jmp") {
      if (track) vm.jmps.push(vm.pc);

      vm.pc += signed_operand;
    } else if (op == "acc") {
      vm.acc += signed_operand;

      vm.pc++;
    }
  }
}

void vm_soft_clear_state(Vm &vm) {
  vm.acc = 0;
  vm.hit_counter.clear();
  vm.pc = 0;
}

int main() {
  fstream input("input.txt");

  Vm vm;

  // Read all instructions.
  string line;
  while (getline(input, line)) vm.instructions.push_back(line);

  bool was_normal_exit = vm_run(vm, true);

  // first exit is bound to be via the infinite loop
  printf("Part one - accumulator: %d\n", vm.acc);

  // Try replacing jmps with nop
  while (!was_normal_exit) {
    int last_jmp = vm.jmps.top();
    vm.jmps.pop();

    // change to nop
    vm.instructions[last_jmp].replace(0, 3, "nop");

    // re-evaluate
    vm_soft_clear_state(vm);
    was_normal_exit = vm_run(vm);

    // revert the change, so we can try the next one.
    vm.instructions[last_jmp].replace(0, 3, "jmp");
  }

  // Try replacing nops with jmp
  while (!was_normal_exit) {
    int last_nop = vm.nops.top();
    vm.nops.pop();

    // change to nop
    vm.instructions[last_nop].replace(0, 3, "jmp");

    // re-evaluate
    vm_soft_clear_state(vm);
    was_normal_exit = vm_run(vm);

    // revert the change, so we can try the next one.
    vm.instructions[last_nop].replace(0, 3, "nop");
  }

  printf("Part two - accumulator: %d\n", vm.acc);
}
