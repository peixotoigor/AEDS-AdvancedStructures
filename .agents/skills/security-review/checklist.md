# Checklist de Auditoria de Segurança

- [ ] Nenhuma chave de API, token JWT secreto ou senha exposta no código-fonte.
- [ ] Todas as consultas SQL ou ORM utilizam binding de parâmetros.
- [ ] Endpoints protegidos exigem verificação do token JWT no middleware.
- [ ] Limitação de taxa (Rate Limiting) aplicada a rotas públicas e autenticação.
- [ ] Sanitização e validação de MIME type em upload de arquivos.
- [ ] Headers de segurança presentes (Content-Security-Policy, X-Frame-Options, HSTS).
