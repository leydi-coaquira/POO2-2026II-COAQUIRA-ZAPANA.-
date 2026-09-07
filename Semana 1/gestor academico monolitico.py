"""
Gestor Academico Monolitico - CODIGO BASE REAL de la guia (seccion 7,
Practica Semana 1, SIS206 POO II, UNAP Puno 2026-II). Transcrito tal
cual del documento; NO modificar (es la linea base para el diagnostico
y la refactorizacion).
"""


class GestorAcademico:
    def __init__(self):
        self.estudiantes = []

    def registrar(self, codigo, nombre):
        self.estudiantes.append({"codigo": codigo, "nombre": nombre, "notas": []})

    def agregar_nota(self, codigo, nota):
        for e in self.estudiantes:
            if e["codigo"] == codigo:
                e["notas"].append(nota)
                with open("auditoria.txt", "a", encoding="utf-8") as f:
                    f.write(f"nota:{codigo}:{nota}\n")

    def reporte(self):
        salida = []
        for e in self.estudiantes:
            promedio = sum(e["notas"]) / len(e["notas"]) if e["notas"] else 0
            estado = "APROBADO" if promedio >= 10.5 else "DESAPROBADO"
            salida.append(f'{e["codigo"]};{e["nombre"]};{promedio:.2f};{estado}')
        with open("reporte.csv", "w", encoding="utf-8") as f:
            f.write("\n".join(salida))
        print("Reporte generado")
if __name__ == "__main__":
                g = GestorAcademico()
                g.registrar("E001", "Ana Quispe")
                g.agregar_nota("E001", 12)
                g.reporte()