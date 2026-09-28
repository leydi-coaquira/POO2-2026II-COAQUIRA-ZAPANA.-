# ── Práctica S3 - Actividad 2: Jerarquía PersonalUniversitario ──────────
import sys
from abc import ABC, abstractmethod

# Para que la consola de Windows muestre bien tildes y símbolos
sys.stdout.reconfigure(encoding="utf-8")


class PersonalUniversitario(ABC):
    """Clase base abstracta para todo el personal de la UNAP."""
    TARIFA_HORA_BASE = 75.0   # S/ por hora

    def __init__(self, dni: str, nombres: str, apellidos: str):
        self.__dni = self._validar_dni(dni)
        self._nombres = nombres
        self._apellidos = apellidos

    @staticmethod
    def _validar_dni(dni: str) -> str:
        if not (dni.isdigit() and len(dni) == 8):
            raise ValueError(f"DNI inválido: {dni!r} (debe tener 8 dígitos)")
        return dni

    @property
    def dni(self) -> str:
        return self.__dni

    @property
    def nombre_completo(self) -> str:
        return f"{self._apellidos}, {self._nombres}"

    @abstractmethod
    def calcular_remuneracion(self) -> float:
        """Cada subclase define su propio cálculo."""
        ...

    def __str__(self) -> str:
        return f"{self.nombre_completo}  ->  S/ {self.calcular_remuneracion():.2f}"


class Docente(PersonalUniversitario):
    FACTORES = {"Auxiliar": 1.0, "Asociado": 1.4, "Principal": 1.8}

    def __init__(self, dni: str, nombres: str, apellidos: str,
                 horas: int, categoria: str):
        super().__init__(dni, nombres, apellidos)
        # Validar horas > 0 y <= 40
        if not (0 < horas <= 40):
            raise ValueError(f"Horas inválidas: {horas} (deben estar entre 1 y 40)")
        # Validar categoria en FACTORES
        if categoria not in self.FACTORES:
            raise ValueError(f"Categoría inválida: {categoria!r} "
                             f"(opciones: {list(self.FACTORES)})")
        # Asignar atributos
        self._horas = horas
        self._categoria = categoria

    def calcular_remuneracion(self) -> float:
        # horas * TARIFA_HORA_BASE * factor de categoría
        return self._horas * self.TARIFA_HORA_BASE * self.FACTORES[self._categoria]


class Administrativo(PersonalUniversitario):
    def __init__(self, dni: str, nombres: str, apellidos: str,
                 salario_base: float, bono: float = 0.0):
        super().__init__(dni, nombres, apellidos)
        # Validar salario_base > 0
        if salario_base <= 0:
            raise ValueError(f"Salario base inválido: {salario_base} (debe ser positivo)")
        # Asignar atributos
        self._salario_base = salario_base
        self._bono = bono

    def calcular_remuneracion(self) -> float:
        # salario base + bono
        return self._salario_base + self._bono


class Investigador(PersonalUniversitario):
    """Investigador a tiempo completo - remunerado por proyectos activos."""
    REMUNERACION_POR_PROYECTO = 1500.0

    def __init__(self, dni: str, nombres: str, apellidos: str,
                 proyectos: int):
        super().__init__(dni, nombres, apellidos)
        if proyectos < 1:
            raise ValueError("Debe tener al menos 1 proyecto activo")
        self._proyectos = proyectos

    def calcular_remuneracion(self) -> float:
        return self._proyectos * self.REMUNERACION_POR_PROYECTO


def planilla(personal: list[PersonalUniversitario]) -> None:
    """Imprime la planilla usando polimorfismo - sin isinstance()."""
    total = 0.0
    print("=" * 60)
    print("PLANILLA PERSONAL UNIVERSITARIO - UNAP PUNO")
    print("=" * 60)
    for p in personal:
        print(p)          # llama __str__ -> calcular_remuneracion polimórfico
        total += p.calcular_remuneracion()
    print("-" * 60)
    print(f"TOTAL PLANILLA:  S/ {total:.2f}")


# ── Pruebas ─────────────────────────────────────────────────────────────
personal = [
    Docente("12345678", "Juan Carlos", "Mamani Quispe", 16, "Asociado"),
    Docente("23456789", "María Elena", "Quispe Torres", 12, "Principal"),
    Administrativo("34567890", "Pedro", "Huanca Lazo", 2800.0, 350.0),
    Investigador("45678901", "Ana Lucía", "Flores Ramos", 2),
]
planilla(personal)

# Verificar LSP - todos son PersonalUniversitario
for p in personal:
    assert isinstance(p, PersonalUniversitario)
    assert p.calcular_remuneracion() > 0
print("\nVerificación LSP: OK - todos los objetos respetan el contrato base")

# ── Pruebas de validación (demuestran que las validaciones funcionan) ───
print("\nPruebas de validación:")
casos_invalidos = [
    lambda: Docente("11111111", "Test", "Horas", 50, "Asociado"),      # horas > 40
    lambda: Docente("11111111", "Test", "Horas", 0, "Asociado"),       # horas = 0
    lambda: Docente("11111111", "Test", "Categoria", 10, "Titular"),   # categoría inválida
    lambda: Administrativo("22222222", "Test", "Salario", -500.0),     # salario negativo
    lambda: Docente("123", "Test", "DNI", 10, "Auxiliar"),             # DNI inválido
]
for crear in casos_invalidos:
    try:
        crear()
        print("  [X] No se detectó el error")
    except ValueError as e:
        print(f"  [OK] Error capturado: {e}")