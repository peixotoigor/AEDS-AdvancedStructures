import fitz

doc = fitz.open('test_fig.pdf')
page = doc[0]
text = page.get_text()
print("Page text:\n", text.encode('ascii', errors='replace').decode('ascii'))
