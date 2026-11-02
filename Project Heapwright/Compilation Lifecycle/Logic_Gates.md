# 🧱 Logic Gates

## 💭 The Idea

We've seen that a transistor is a switch that can be turned **on or off**. We write **ON as 1** and **OFF as 0**.

These 0s and 1s make up a number system called **binary**, the fundamental language of computers. Each single 0 or 1 is called a **bit**, and a bit is the smallest unit of information a computer can store.

But computers don't only *store* information, they **process** it. Inputs go in, and a different output comes out.

```text
   inputs ──►  [ LOGIC GATE ]  ──► output
```

This is where **logic gates** come in.

> A **logic gate** takes in one or more inputs and gives back an output, following a fixed logical rule.

We'll cover six gates:

| Group | Gates |
|-------|-------|
| **The basics** | NOT, AND, OR |
| **Their "opposite" versions** | NAND, XNOR |
| **The "one or the other" gate** | XOR |

---

## 🔁 NOT Gate

The NOT gate takes **one input**, either a 0 or a 1, and gives back the **opposite**.

- If the input is **1**, the output is **0**.
- If the input is **0**, the output is **1**.

It never keeps the input the same. It always turns it around, which is why it's also called an **inverter**.

**Symbol:** a triangle with a small circle on the end. The circle is the "flip".

```text
  A ───▷o─── Output
```

**Truth table** (a list of every possible input and what comes out):

| Input | Output |
|:-----:|:------:|
| 0 | 1 |
| 1 | 0 |

💡 **Bulb example:** imagine a bulb that is **on by default**, with a special button that **turns it off when pressed**. Not pressed (0) gives light (1). Pressed (1) gives no light (0).

---

## 🤝 AND Gate

Often a computer needs to make a decision based on **more than one bit**. The AND gate does this with **two inputs**. Let's call them **A** and **B**.

- If **both** A and B are 1, the output is **1**.
- In **every other case**, the output is **0**.

So the gate only says "yes" when *everything* agrees. If even one input is 0, the answer is 0.

**Symbol:** flat on the back, rounded on the front.

```text
  A ──┐‾‾‾╲
      │    )── Output
  B ──┘___╱
```

**Truth table:**

| A | B | Output |
|:-:|:-:|:------:|
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | **1** |

💡 **Bulb example:** two switches placed **one after the other** on the same wire. The electricity has to pass through both, so the bulb only lights when **switch A and switch B are both closed**. Open either one and the light goes out.

```text
  battery ──[ A ]──[ B ]──( bulb )
```

---

## 🔀 OR Gate

The OR gate also has **two inputs**, A and B, but it's more relaxed than AND.

- If **at least one** input is 1, the output is **1**. That includes the case where both are 1.
- The output is **0 only when both inputs are 0**.

So the gate says "yes" as soon as *anything* says yes.

**Symbol:** curved on the back, pointed on the front.

```text
  A ──╮‾‾‾╲
      )    )── Output
  B ──╯___╱
```

**Truth table:**

| A | B | Output |
|:-:|:-:|:------:|
| 0 | 0 | 0 |
| 0 | 1 | **1** |
| 1 | 0 | **1** |
| 1 | 1 | **1** |

💡 **Bulb example:** two switches on **separate wires, side by side**. The electricity can take either route, so the bulb lights when **switch A or switch B (or both) is closed**. It only stays dark when both are open.

```text
          ┌──[ A ]──┐
  battery ┤         ├──( bulb )
          └──[ B ]──┘
```

---

## 🚫 NAND Gate

**NAND** means **NOT AND**. Take an AND gate and flip its answer with a NOT.

- If **both** inputs are 1, the output is **0**.
- In **every other case**, the output is **1**.

It's exactly the opposite of AND. Compare the two tables and every row is flipped.

**Symbol:** the AND shape with a small circle on the end (the same "flip" circle as the NOT gate).

```text
  A ──┐‾‾‾╲
      │    )o── Output
  B ──┘___╱
```

**Truth table:**

| A | B | AND | NAND |
|:-:|:-:|:---:|:----:|
| 0 | 0 | 0 | **1** |
| 0 | 1 | 0 | **1** |
| 1 | 0 | 0 | **1** |
| 1 | 1 | 1 | **0** |

💡 **Bulb example:** the same two switches in a row as AND, but the bulb is wired to be **on by default**. It only goes **out** when **both** switches are pressed.

> 📝 NAND is a very important gate. Any other gate can be built out of NAND gates alone, which is why chips use it so much.

---

## ⚡ XOR Gate

**XOR** means **eXclusive OR**: "one or the other, **but not both**."

- If the inputs are **different** (one is 1, the other is 0), the output is **1**.
- If the inputs are the **same** (both 0 or both 1), the output is **0**.

Remember, OR says yes even when both inputs are 1. XOR changes that. The only difference from OR is the last row: when both are 1, OR gives 1 but XOR gives 0.

**Symbol:** the OR shape with an **extra curved line** on the back.

```text
  A ──╮‾‾‾╲
     ))    )── Output
  B ──╯___╱
```

**Truth table:**

| A | B | OR | XOR |
|:-:|:-:|:--:|:---:|
| 0 | 0 | 0 | 0 |
| 0 | 1 | 1 | **1** |
| 1 | 0 | 1 | **1** |
| 1 | 1 | 1 | **0** |

💡 **Bulb example:** the **staircase light**. There's a switch at the bottom of the stairs (A) and one at the top (B). Flip either one and the light changes. If the two switches are in **different** positions the light is on. If they're in the **same** position, it's off. Flipping just one switch always changes the light.

---

## 🟰 XNOR Gate

**XNOR** means **NOT XOR**. It flips XOR's answer.

- If the inputs are the **same** (both 0 or both 1), the output is **1**.
- If the inputs are **different**, the output is **0**.

Think of it as an **"are these equal?"** gate. It says 1 when A and B match.

**Symbol:** the XOR shape with a small circle on the end.

```text
  A ──╮‾‾‾╲
     ))    )o── Output
  B ──╯___╱
```

**Truth table:**

| A | B | XOR | XNOR |
|:-:|:-:|:---:|:----:|
| 0 | 0 | 0 | **1** |
| 0 | 1 | 1 | **0** |
| 1 | 0 | 1 | **0** |
| 1 | 1 | 0 | **1** |

💡 **Bulb example:** the same staircase switches as XOR, but now the light is **on when both switches are in the same position** and **off when they differ**.

---

## 📋 Summary

| Gate | Inputs | Output is 1 when… | Easy way to remember |
|------|:------:|-------------------|----------------------|
| **NOT** | 1 | the input is 0 | Flip it |
| **AND** | 2 | **both** are 1 | Everyone agrees |
| **OR** | 2 | **at least one** is 1 | Anyone says yes |
| **NAND** | 2 | **not both** are 1 | Opposite of AND |
| **XOR** | 2 | the inputs are **different** | One or the other, not both |
| **XNOR** | 2 | the inputs are the **same** | Opposite of XOR ("are they equal?") |

### All truth tables in one place

| A | B | AND | OR | NAND | XOR | XNOR |
|:-:|:-:|:---:|:--:|:----:|:---:|:----:|
| 0 | 0 | 0 | 0 | 1 | 0 | 1 |
| 0 | 1 | 0 | 1 | 1 | 1 | 0 |
| 1 | 0 | 0 | 1 | 1 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 | 1 |

(NOT isn't in this table because it only has one input.)
