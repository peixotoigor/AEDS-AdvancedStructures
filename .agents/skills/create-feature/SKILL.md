---
name: create-feature
description: Cria uma nova funcionalidade completa seguindo Clean Architecture, TDD e a configuração deste repositório
---

# Skill: Create Feature (`/create-feature`)

Esta skill orienta a implementação estruturada de uma nova funcionalidade.

## Diretrizes Mandatórias
1. **Consulte o repositório:** Confirme no código, testes e configurações versionadas os padrões ativos antes de implementar.
2. **Ciclo de Desenvolvimento (Clean Architecture):**
   - **Camada 1 - Domínio (`src/domain/`):** Crie a entidade ou value object com validação de invariantes.
   - **Camada 2 - Aplicação (`src/application/`):** Defina a interface de repositório (Port), crie DTOs, teste unitário (Vitest AAA) e implemente o Use Case.
   - **Camada 3 - Infraestrutura (`src/infrastructure/`):** Implemente o repositório com o ORM selecionado.
   - **Camada 4 - Apresentação (`src/presentation/`):** Validação Zod, Controller e rotas com `{ success, data }`.
3. **Verificação Automática:** Executar lint e testes automatizados antes de concluir.
