# ── errores_academicos.py — jerarquía equivalente Python ────────────────
import sys
try:
    sys.stdout.reconfigure(encoding="utf-8")
except AttributeError:
    pass  # por si corre en un entorno que ya maneja utf-8 (p. ej. Colab)


class ErrorAcademico(Exception):
    """Raíz de todas las excepciones del dominio académico."""
    def __init__(self, mensaje: str, contexto: dict | None = None):
        super().__init__(mensaje)
        self.contexto: dict = contexto or {}

    def __str__(self) -> str:
        base = super().__str__()
        if self.contexto:
            ctx = ", ".join(f"{k}={v!r}" for k, v in self.contexto.items())
            return f"{base}  ← contexto: {{{ctx}}}"
        return base
 
class ErrorValidacion(ErrorAcademico):
    """Datos de entrada que violan las reglas del dominio."""
 
 
class ErrorCalificacion(ErrorValidacion):
    """Nota fuera del rango [0, 20] del sistema peruano vigesimal."""
    def __init__(self, valor: float, campo: str = "nota"):
        super().__init__(
            f"Calificación {valor} fuera de [0, 20]",
            contexto={"campo": campo, "valor": valor, "rango": "[0, 20]"}
        )
        self.valor_invalido = valor
 
 
class ErrorCapacidad(ErrorAcademico):
    """Operación que excede el límite permitido."""
    def __init__(self, actual: int, maximo: int, entidad: str):
        super().__init__(
            f"{entidad}: capacidad agotada ({actual}/{maximo})",
            contexto={"actual": actual, "maximo": maximo, "entidad": entidad}
        )
 
 
class ErrorRecursoNoEncontrado(ErrorAcademico):
    """Entidad buscada que no existe en el repositorio."""
    def __init__(self, tipo: str, identificador):
        super().__init__(
            f"{tipo} con id={identificador!r} no encontrado",
            contexto={"tipo": tipo, "id": identificador}
        )
 
 
# ── Demostración de captura y encadenamiento ─────────────────────────────
def registrar_nota(valor: float, campo: str = "nota") -> None:
    if not (0.0 <= valor <= 20.0):
        raise ErrorCalificacion(valor, campo)
 
try:
    registrar_nota(25.0, "POO II")
except ErrorCalificacion as e:
    print(f"Error específico: {e}")
except ErrorValidacion as e:    # captura si no fue ErrorCalificacion
    print(f"Error de validación: {e}")
except ErrorAcademico as e:
    print(f"Error de dominio: {e}")
 
# ── Encadenamiento from: preserva causa original ─────────────────────────
def cargar_nota_desde_archivo(ruta: str, codigo: int) -> float:
    try:
        with open(ruta) as f:
            return float(f.read())
    except (IOError, ValueError) as causa:
        raise ErrorRecursoNoEncontrado("Nota", codigo) from causa
        # ↑ __cause__ = causa original; visible en el traceback
