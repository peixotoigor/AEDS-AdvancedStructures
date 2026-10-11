# Agent Instructions

These are repository-wide instructions for AI coding agents and automated
development harnesses. They are tool-agnostic and apply to planning,
implementation, review, testing and documentation work in any project.

## Priority of instructions

Follow this order when instructions conflict:

1. System, platform and user instructions.
2. This file.
3. Existing code conventions and surrounding implementation.

Do not invent project conventions when the repository already has an applicable
pattern.

## Academic project constraints

Read `docs/requirements.md` before changing the academic scope, deliverables or
report requirements.

- Do not invent benchmark results, measurements, references, experimental data
  or conclusions.
- Use only values supported by repository files, executed commands or recorded
  experiments.
- Keep explanations and implementations clear enough for an undergraduate
  technical presentation.

## Repository documentation

Read only the documentation relevant to the task:

- `README.md`: read first for project overview, structure and usage.
- `docs/requirements.md`: read before changing project scope, deliverables or
  acceptance criteria.
- `docs/architecture.md`: read before changing structure, data representation,
  interfaces or component contracts.
- `docs/coding-style.md`: read before writing or modifying implementation code.
- `docs/latex-writing-style.md`: read before writing or reviewing the academic
  report, LaTeX figures, tables or algorithms.
- `docs/development.md`: read before changing workflow, commands or experiments.
- `docs/testing.md`: read before adding or modifying tests or validation.
- `docs/benchmarks.md`: read before creating or changing performance
  measurements, CSV output or result analysis.

When a task crosses multiple areas, read all corresponding documents before
editing. Do not load unrelated documentation unnecessarily.

## Before editing

- Inspect the relevant files, tests and build configuration.
- Confirm the requested behavior and identify affected surfaces.
- Keep the change focused; do not rewrite unrelated code.
- Preserve uncommitted work that was not created by you.
- Ask for clarification when multiple designs have materially different
  behavior or scope.

## Implementation rules

- Follow the language, framework and architecture already used by the project.
- Prefer simple, readable solutions over unnecessary abstractions.
- Reuse existing helpers, utilities and conventions before adding new ones.
- Preserve invariants after every operation.
- Prevent resource leaks, invalid state, unsafe access and undefined behavior.
- Preserve type safety and use explicit error handling.
- Document non-obvious decisions, public behavior and compatibility concerns.
- Avoid adding dependencies unless they are necessary and documented.

## Tests and validation

Every behavior change must have appropriate tests. Cover, when applicable:

- empty input;
- one element;
- normal input;
- repeated values or keys;
- missing values;
- boundary values;
- insertion, removal and repeated operations;
- comparison with a trusted reference or equivalent implementation.

Use deterministic inputs. Record a random seed when randomness is necessary.
Run the smallest relevant validation first, then expand when the change affects
shared behavior.

Preferred validation order:

1. format and compile;
2. focused tests;
3. full tests;
4. AddressSanitizer and UndefinedBehaviorSanitizer when memory safety is
   relevant;
5. benchmark or experiment commands when performance is part of the change.

Never hide failures with broad exception handling, silent fallbacks or
success-shaped output.

## Consistency across implementation and report

When changing an algorithm or its behavior:

- update the relevant pseudocode, complexity analysis, tests, experiments,
  tables, figures and conclusions when necessary;
- ensure documented behavior matches the implementation;
- distinguish worst-case, average-case and amortized complexity;
- do not claim correctness or performance based only on an unverified
  benchmark.

Before modifying tests or validation, read `docs/testing.md`. Before creating
or changing performance experiments, read `docs/benchmarks.md`.

## Documentation and experiments

Update directly related documentation when behavior, commands, complexity or
project structure changes. Keep examples, specifications, complexity analysis
and experimental results consistent with the implementation.

Before writing or reviewing the academic report, LaTeX figures, tables or
algorithms, read `docs/latex-writing-style.md`.

- Cite every figure, table and algorithm in the text.
- Use labels and informative captions.
- Do not invent bibliographic references or experimental values.
- Preserve the source data used to generate charts and tables.

## Commands

Use the commands defined by the repository. Inspect package manifests,
workflows, task definitions and documentation to find the canonical commands.
If a required build file, test target or dependency is missing, report it
explicitly and add only the minimum infrastructure needed.

## Completion checklist

Before finishing:

- confirm the requested behavior is implemented;
- inspect the final diff;
- run relevant validation;
- report tests or checks that passed and any limitations;
- do not commit, push or perform destructive operations unless explicitly
  requested.
