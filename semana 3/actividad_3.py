# Actividad 3 Verificación del LSP y Test de sustitución 
# Indicaciones:
#  Implementar verificar_lsp(): dado cualquier PersonalUniversitario,
#   ejecuta un contrato invariante sobre él.
#  Demostrar que TODAS las subclases lo pasan.
#  Implementar la jerarquía Figura -> Rectangulo -> Cuadrado y explicar
#   por qué Cuadrado VIOLA el LSP.

import sys
from abc import ABC, abstractmethod

sys.stdout.reconfigure(encoding="utf-8")   # para la consola de Windows

# Reutilizamos las clases y la lista personal de la Actividad 2
# actividad_2.py debe estar en la misma carpeta
from actividad_2 import PersonalUniversitario, personal


# 1 Contrato invariante para la jerarquía PersonalUniversitario
def verificar_contrato_personal(p: PersonalUniversitario) -> bool:
    """
    Contrato que TODA subclase debe satisfacer
    nombre_completo retorna string no vacío
    calcular_remuneracion() retorna valor > 0
    str(p) contiene el nombre completo
    """
    ok = True
    ok = ok and isinstance(p.nombre_completo, str) and len(p.nombre_completo) > 0
    ok = ok and p.calcular_remuneracion() > 0
    ok = ok and p.nombre_completo in str(p)
    return ok


def verificar_lsp(lista: list[PersonalUniversitario]) -> bool:
    """Ejecuta el contrato sobre cada objeto e imprime el resultado."""
    todos_ok = True
    for p in lista:
        cumple = verificar_contrato_personal(p)
        tipo = type(p).__name__          # Docente, Administrativo, Investigador
        print(f"{tipo:<15} {p.nombre_completo:<30} -> {'OK' if cumple else 'FALLA'}")
        todos_ok = todos_ok and cumple
    return todos_ok


# 2 Demostrar que TODAS las subclases pasan el contrato
print("\n" + "=" * 60)
print("ACTIVIDAD 3 - VERIFICACIÓN DEL CONTRATO LSP")
print("=" * 60)
resultado = verificar_lsp(personal)
print("-" * 60)
if resultado:
    print("Todas las subclases cumplen el LSP")
else:
    print("Alguna subclase NO cumple el LSP")


# 3 Violación clásica de LSP, Cuadrado hereda de Rectangulo
class Figura:
    def area(self) -> float: ...


class Rectangulo(Figura):
    def __init__(self, ancho: float, alto: float):
        self._ancho = ancho
        self._alto = alto

    def set_ancho(self, v: float) -> None: self._ancho = v
    def set_alto(self, v: float) -> None: self._alto = v
    def area(self) -> float: return self._ancho * self._alto


class Cuadrado(Rectangulo):
    def __init__(self, lado: float):
        super().__init__(lado, lado)

    def set_ancho(self, v: float) -> None:
        self._ancho = v
        self._alto = v    # fuerza ambos lados iguales

    def set_alto(self, v: float) -> None:
        self._ancho = v
        self._alto = v


def test_rectangulo(r: Rectangulo) -> None:
    """Test que DEBERÍA pasar para cualquier Rectangulo según LSP."""
    r.set_ancho(4)
    r.set_alto(5)
    area_esperada = 20.0
    assert r.area() == area_esperada, \
        f"LSP violado: area={r.area()}, esperado={area_esperada}"


print("\n" + "=" * 60)
print("VIOLACIÓN DEL LSP: Cuadrado hereda de Rectangulo")
print("=" * 60)

rect = Rectangulo(2, 2)
test_rectangulo(rect)    # OK
print("Rectangulo: test OK (area = 20)")

cuad = Cuadrado(2)
try:
    test_rectangulo(cuad)    # FALLA viola LSP
except AssertionError as e:
    print(f"LSP VIOLADO en Cuadrado: {e}")
    print("Solución: no heredar Cuadrado de Rectangulo; "
          "usar composición o interfaz independiente.")


# 4 Alternativa de diseño que sí respeta el LSP
#    Rectangulo y Cuadrado son hermanos, ambos heredan de una figura
#    abstracta y ninguno hereda del otro.
class FiguraGeometrica(ABC):
    @abstractmethod
    def area(self) -> float: ...


class RectanguloLSP(FiguraGeometrica):
    def __init__(self, ancho: float, alto: float):
        self._ancho = ancho
        self._alto = alto

    def area(self) -> float:
        return self._ancho * self._alto


class CuadradoLSP(FiguraGeometrica):
    def __init__(self, lado: float):
        self._lado = lado

    def area(self) -> float:
        return self._lado ** 2


print("\n" + "=" * 60)
print("ALTERNATIVA: Rectangulo y Cuadrado como clases hermanas")
print("=" * 60)
figuras: list[FiguraGeometrica] = [RectanguloLSP(4, 5), CuadradoLSP(5)]
for f in figuras:
    assert f.area() > 0    # contrato de FiguraGeometrica
    print(f"{type(f).__name__:<15} area = {f.area():.2f}")
print("Ninguna figura rompe el contrato de la clase base -> LSP cumplido")