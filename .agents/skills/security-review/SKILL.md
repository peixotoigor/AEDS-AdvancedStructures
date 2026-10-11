---
name: security-review
description: "Auditoria e revisão minuciosa de segurança contra vulnerabilidades OWASP, injeção e vazamento de credenciais"
invocation: agent_or_user
---

# Skill: Revisão de Segurança (Security Review)

## Quando Utilizar
Carregue esta skill sempre que estiver alterando lógicas de autenticação, permissões, queries de banco de dados, upload de arquivos ou manipulação de headers.

## Procedimento de Execução
1. **Inspeção de Diffs:** Analise as alterações comparando com a checklist de segurança anexada.
2. **Carregamento de Recursos:** Leia detalhadamente o arquivo `.agents/skills/security-review/checklist.md`.
3. **Verificação de Vulnerabilidades:**
   - Verifique ausência de segredos ou chaves privadas (.env commitado).
   - Verifique sanitização de inputs contra XSS e SQL Injection.
   - Verifique controle de acesso baseado em papéis (RBAC).
4. **Relatório:** Produza um artefato estruturado listando vulnerabilidades encontradas, nível de severidade (Baixo, Médio, Alto, Crítico) e plano de mitigação.
