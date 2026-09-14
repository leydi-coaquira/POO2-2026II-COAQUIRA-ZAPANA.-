"""
Practica S2 - Actividad 5
Proyecto Integrador: Sistema de Monitoreo de Sensores IoT del Lago Titicaca.
Sensor, EstacionMonitoreo y Lectura.

"""

import re
from datetime import datetime


class Sensor:
    _PATRON_CODIGO = re.compile(r"^SEN\d{3}$")
    _TIPOS_VALIDOS = {"temperatura", "turbidez", "nivel"}
    _RANGOS_FISICOS = {
        "temperatura": (-5.0, 40.0),   # grados Celsius, plausibles en el lago
        "turbidez": (0.0, 1000.0),     # NTU
        "nivel": (0.0, 50.0),          # metros
    }
    _UNIDADES = {"temperatura": "°C", "turbidez": "NTU", "nivel": "m"}

    def __init__(self, codigo: str, tipo: str, ubicacion: str):
        self.codigo = codigo        # usa setter -> valida
        self.tipo = tipo            # usa setter -> valida
        self.ubicacion = ubicacion  # usa setter -> valida

    @property
    def codigo(self) -> str:
        return self.__codigo

    @codigo.setter
    def codigo(self, valor: str) -> None:
        if not self._PATRON_CODIGO.match(valor):
            raise ValueError(f"Codigo de sensor invalido: '{valor}' (formato SEN+3 digitos)")
        self.__codigo = valor

    @property
    def tipo(self) -> str:
        return self.__tipo

    @tipo.setter
    def tipo(self, valor: str) -> None:
        if valor not in self._TIPOS_VALIDOS:
            raise ValueError(f"Tipo de sensor invalido: '{valor}' (opciones: {self._TIPOS_VALIDOS})")
        self.__tipo = valor

    @property
    def ubicacion(self) -> str:
        return self.__ubicacion

    @ubicacion.setter
    def ubicacion(self, valor: str) -> None:
        if len(valor.strip()) < 3:
            raise ValueError("La ubicacion debe tener al menos 3 caracteres")
        self.__ubicacion = valor.strip()

    @property
    def unidad_medida(self) -> str:
        """Propiedad CALCULADA -- se deriva del tipo, no se almacena aparte."""
        return self._UNIDADES[self.__tipo]

    def validar_rango(self, valor: float) -> bool:
        """Metodo de negocio: verifica la precondicion de que el valor sea
        fisicamente plausible para este tipo de sensor antes de aceptarlo."""
        if not isinstance(valor, (int, float)):
            raise TypeError("El valor de la lectura debe ser numerico")
        minimo, maximo = self._RANGOS_FISICOS[self.__tipo]
        if valor < minimo or valor > maximo:
            raise ValueError(
                f"Valor {valor}{self.unidad_medida} fuera del rango fisico "
                f"esperado para '{self.__tipo}' [{minimo},{maximo}]"
            )
        return True

    def __str__(self) -> str:
        return f"{self.__codigo} ({self.__tipo}, {self.unidad_medida}) @ {self.__ubicacion}"


class EstacionMonitoreo:
    _PATRON_ID = re.compile(r"^EST\d{2}$")
    _MAX_SENSORES = 10

    def __init__(self, id_estacion: str, nombre: str):
        self.id_estacion = id_estacion  # usa setter -> valida
        self.nombre = nombre            # usa setter -> valida
        self.__sensores: list[Sensor] = []

    @property
    def id_estacion(self) -> str:
        return self.__id_estacion

    @id_estacion.setter
    def id_estacion(self, valor: str) -> None:
        if not self._PATRON_ID.match(valor):
            raise ValueError(f"Id de estacion invalido: '{valor}' (formato EST+2 digitos)")
        self.__id_estacion = valor

    @property
    def nombre(self) -> str:
        return self.__nombre

    @nombre.setter
    def nombre(self, valor: str) -> None:
        if not valor.strip():
            raise ValueError("El nombre de la estacion no puede estar vacio")
        self.__nombre = valor.strip()

    @property
    def total_sensores(self) -> int:
        """Propiedad CALCULADA a partir de la lista interna de sensores."""
        return len(self.__sensores)

    @property
    def tiene_capacidad(self) -> bool:
        """Propiedad calculada booleana."""
        return self.total_sensores < self._MAX_SENSORES

    def agregar_sensor(self, sensor: Sensor) -> None:
        """Metodo de negocio: verifica precondiciones (capacidad y no
        duplicados) antes de modificar el estado interno."""
        if not self.tiene_capacidad:
            raise RuntimeError(
                f"La estacion {self.__id_estacion} ya tiene el maximo de "
                f"{self._MAX_SENSORES} sensores"
            )
        if any(s.codigo == sensor.codigo for s in self.__sensores):
            raise ValueError(f"El sensor {sensor.codigo} ya esta registrado en esta estacion")
        self.__sensores.append(sensor)

    def __str__(self) -> str:
        return (
            f"{self.__id_estacion} - {self.__nombre} "
            f"({self.total_sensores}/{self._MAX_SENSORES} sensores)"
        )


class Lectura:
    def __init__(self, fecha_hora: datetime, valor: float, sensor_id: str):
        self.fecha_hora = fecha_hora  # usa setter -> valida
        self.__valor = float(valor)
        self.sensor_id = sensor_id    # usa setter -> valida

    @property
    def fecha_hora(self) -> datetime:
        return self.__fecha_hora

    @fecha_hora.setter
    def fecha_hora(self, valor: datetime) -> None:
        if valor > datetime.now():
            raise ValueError("La fecha_hora de una lectura no puede estar en el futuro")
        self.__fecha_hora = valor

    @property
    def valor(self) -> float:
        return self.__valor

    @property
    def sensor_id(self) -> str:
        return self.__sensor_id

    @sensor_id.setter
    def sensor_id(self, valor: str) -> None:
        if not valor.strip():
            raise ValueError("sensor_id no puede estar vacio")
        self.__sensor_id = valor.strip()

    @property
    def antiguedad_horas(self) -> float:
        """Propiedad CALCULADA -- se recalcula cada vez que se consulta."""
        return (datetime.now() - self.__fecha_hora).total_seconds() / 3600

    def verificar_con_sensor(self, sensor: Sensor) -> bool:
        """Metodo de negocio: exige como precondicion que la lectura
        pertenezca al sensor indicado antes de validar el valor contra
        el rango fisico de ese sensor."""
        if sensor.codigo != self.__sensor_id:
            raise ValueError(
                f"La lectura pertenece al sensor {self.__sensor_id}, no a {sensor.codigo}"
            )
        return sensor.validar_rango(self.__valor)

    def __str__(self) -> str:
        return f"[{self.__fecha_hora:%Y-%m-%d %H:%M}] {self.__sensor_id} = {self.__valor}"


if __name__ == "__main__":
    # --- Pruebas ---
    est = EstacionMonitoreo("EST01", "Bahia de Huata")
    s1 = Sensor("SEN001", "temperatura", "Orilla - Huata")
    s2 = Sensor("SEN002", "turbidez", "Zona de totorales")
    est.agregar_sensor(s1)
    est.agregar_sensor(s2)
    print(est)
    print(s1, "|", s1.unidad_medida)

    lect = Lectura(datetime.now(), 14.5, "SEN001")
    print(lect, "-> valida:", lect.verificar_con_sensor(s1))
    print(f"Antiguedad: {lect.antiguedad_horas:.4f} h")

    # Invariantes -- deben lanzar error
    try:
        Sensor("BAD", "temperatura", "x")
    except ValueError as ex:
        print(f"Error esperado: {ex}")
    try:
        Sensor("SEN003", "ph", "Orilla")
    except ValueError as ex:
        print(f"Error esperado: {ex}")
    try: 
        est.agregar_sensor(s1)  # duplicado
    except ValueError as ex:
        print(f"Error esperado: {ex}")
    try:
        lect_mala = Lectura(datetime.now(), 999, "SEN001")
        lect_mala.verificar_con_sensor(s1)  # fuera de rango fisico
    except ValueError as ex:
        print(f"Error esperado: {ex}")
    try:
        lect2 = Lectura(datetime.now(), 10, "SEN002")
        lect2.verificar_con_sensor(s1)  # sensor incorrecto
    except ValueError as ex:
        print(f"Error esperado: {ex}")