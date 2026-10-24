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

There are three basic gates: **NOT**, **AND** and **OR**.

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

## 📋 Summary

| Gate | Inputs | Output is 1 when… |
|------|:------:|-------------------|
| **NOT** | 1 | the input is 0 (it flips it) |
| **AND** | 2 | **both** inputs are 1 |
| **OR** | 2 | **at least one** input is 1 |
