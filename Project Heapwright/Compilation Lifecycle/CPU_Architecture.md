# 🧠 CPU Logic: From Code to Switches

> **The big picture:** Your C code is compiled into machine instructions, and those instructions are executed by a CPU made of **billions of tiny switches**. This note covers the CPU side: what those switches are and how they behave.

```text
  C code ──► machine code (1s and 0s) ──► CPU ──► transistors switching ON/OFF
                                                      ▲
                                            this is where we're going
```

> ➡️ **Next note:** [Logic Gates](./logic_gates.md), where these switches get combined into decisions.

---

## 📑 Contents

1. [🔄 Compilation Lifecycle Recap](#-compilation-lifecycle-recap)
2. [🔌 The Transistor: one switch](#-the-transistor-one-switch)
3. [⚠️ Switch Vocabulary](#️-switch-vocabulary-a-common-trap)
4. [👯 N-Type & P-Type: the opposite twins](#-n-type--p-type-the-opposite-twins)
5. [📋 Cheat Sheet](#-cheat-sheet)

---

## 🔄 Compilation Lifecycle Recap

- A program goes from **source code** to **CPU instructions** through the compiler.
- The CPU doesn't understand C. It only understands **1s and 0s**.
- Those 1s and 0s are physically **voltages** (on / off) flowing through the CPU.
- So the real question is: *what is the CPU made of that can represent and act on 1s and 0s?*

> **Answer:** transistors.

---

## 🔌 The Transistor: one switch

- The CPU is made of **transistors** that turn **ON and OFF** (1 and 0).
- A **transistor** is an electrical component that changes state from on to off and back again.

> 🎯 **Its one job:** switching electrical signals on and off.

- A single transistor can't do much on its own. The power comes from **chaining many together** (that's the next note).

```text
   1 transistor      →  a switch                  (this note)
   2–6 transistors   →  a gate, a decision        (next note)
   millions of gates →  an ALU, registers, a CPU
```

---

## ⚠️ Switch Vocabulary (a common trap)

| Term | Bridge | Electricity | Meaning |
|------|--------|-------------|---------|
| **CLOSED** switch | Down 🌉 | Flows | 🟢 CONNECTED / ON |
| **OPEN** switch | Up 🔼 | Blocked | 🔴 DISCONNECTED / OFF |

It feels backwards, but "closed" means the circuit is *complete*.

---

## 👯 N-Type & P-Type: the opposite twins

Transistors come in two flavours that behave **oppositely**. A control wire called the **input** (think of it as a lever) tells each one when to open or close.

| Type | Input = 1 (voltage) | Input = 0 (no voltage) |
|------|---------------------|------------------------|
| **N-Type** | 🟢 CLOSES (passes) | 🔴 OPENS (blocks) |
| **P-Type** | 🔴 OPENS (blocks) | 🟢 CLOSES (passes) |

> 🧠 **Memory trick:** **N** = "**N**ormal": voltage turns it **on**. **P** = "**P**-opposite": voltage turns it **off**.

### Why have both?

- The two types are good at different jobs:
  - **P-Type** is good at connecting something to the **voltage rail** (pulling it to 1).
  - **N-Type** is good at connecting something to **ground** (pulling it to 0).
- Pair them up and you can drive a wire firmly to **either** 1 or 0. That pairing is called **CMOS**, the technology inside nearly every modern CPU.

---

## 📋 Cheat Sheet

```text
  TRANSISTOR   →  an electrically controlled switch (ON/OFF = 1/0)
  CLOSED       →  🟢 connected, electricity flows
  OPEN         →  🔴 disconnected, electricity blocked
  N-TYPE       →  input 1 = closed,  input 0 = open
  P-TYPE       →  input 1 = open,    input 0 = closed
  CMOS         →  N-type + P-type working as a pair
```

> ➡️ Continue to [Logic Gates](./Logic_Gates.md).
