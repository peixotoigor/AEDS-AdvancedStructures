---
name: db-migration
description: Procedimento seguro para planejamento, aprovação e execução de migrações de banco de dados
---

# Skill: Database Migration (`/db-migration`)

> [!CAUTION]
> **REGRA DE OURO:** NUNCA execute alterações de banco de dados (`npx prisma migrate dev`, DDL ou migrations) sem apresentar o diff prévio e obter autorização expressa do desenvolvedor humano.

## Procedimento Seguro
1. **Verificação de Configuração:** Consulte `PROJECT_CONFIG.md` e confirme o banco, ORM e migrations nos arquivos versionados do repositório.
2. **Proposta de Mudança:** Apresente o diff e impacto em relacionamentos e dados existentes.
3. **Solicitação de Aprovação:** Aguarde autorização formal do humano.
4. **Execução Após Aprovação:** Execute a migração e regenere os clientes de banco.
5. **Auditoria Pós-Migração:** Valide a suíte de testes.
