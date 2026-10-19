# ── result.py — patrón Result con Generic[T] en Python ──────────────────
from dataclasses import dataclass
from typing import TypeVar, Generic, Callable
 
T = TypeVar("T")
 
@dataclass(frozen=True)
class Result(Generic[T]):
    _valor:  T   | None = None
    _error:  str | None = None
 
    @staticmethod
    def ok(valor: T)    -> "Result[T]": return Result(_valor=valor)
    @staticmethod
    def fallo(e: str)   -> "Result[T]": return Result(_error=e)
 
    @property
    def es_exito(self) -> bool: return self._error is None
 
    @property
    def valor(self) -> T:
        if not self.es_exito:
            raise RuntimeError(f"Result fallido: {self._error}")
        return self._valor
 
    @property
    def error(self) -> str:
        return self._error or ""
 
    def map(self, f: Callable[[T], "Result"]) -> "Result":
        """Encadenar: aplica f solo si el resultado es exitoso."""
        return f(self._valor) if self.es_exito else self
 
 
# ── Pipeline encadenado con map() ────────────────────────────────────────
_db: dict[int, dict] = {1001: {"nombre": "Juan Mamani", "ppa": 15.0}}
 
def buscar_estudiante(codigo: int) -> Result[dict]:
    est = _db.get(codigo)
    return Result.ok(est) if est else Result.fallo(f"Código {codigo} no existe")
 
def validar_ppa_beca(est: dict) -> Result[dict]:
    if est["ppa"] < 14.0:
        return Result.fallo(f"PPA {est["ppa"]} < 14.0 (mínimo para beca)")
    return Result.ok(est)
 
def asignar_beca(est: dict) -> Result[str]:
    return Result.ok(f"Beca asignada: {est["nombre"]}")
 
# Encadenamiento — solo requiere manejo del error al final
resultado = (buscar_estudiante(1001)
             .map(validar_ppa_beca)
             .map(asignar_beca))
 
if resultado.es_exito:
    print(resultado.valor)
else:
    print(f"No se pudo asignar beca: {resultado.error}")
