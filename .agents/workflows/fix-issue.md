# Workflow: Fix Issue (`/fix-issue`)

## Objetivo
Resolver issues, incidentes ou falhas de testes de forma determinística e verificada no Antigravity.

## Passos Operacionais
1. **Reprodução:** Execute o comando de teste afetado para reproduzir a falha e isolar a causa raiz.
2. **Implementação Mínima:** Altere apenas o código estritamente necessário para corrigir o problema.
3. **Verificação Automática:**
   ```bash
   npm run lint
   npm test
   ```
4. **Auditoria:** Invoque o subagente `code-reviewer` para certificar a correção.
5. **Artefato de Entrega:** Produza um resumo da alteração detalhando a causa raiz e a solução aplicada.
