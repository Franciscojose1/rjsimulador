<?php

class CalculateAverageData implements CalculatedDataInterface {
  const CONSUMO_MEDIO = 0;
  const TIEMPO_TOTAL = 1;
  const VELOCIDAD = 2;
  const RPM = 3;

  private $field;

  /**
   * @param int $field Debe ser una de las constantes de la clase CalculateAverageData.
   */
  public function __construct($field) {
    $this->field = $field;
  }

  /**
   * @param Lista $lista
   * @return mixed Valor de la media del campo pasado en el constructor.
   * @throws Exception Si el campo pasado no es admisible para el tipo de lista o si la lista no es un tipo soportado.
   */
  public function calculate(Lista $lista) {
    if ($lista instanceof ListaPartidas) {
      switch ($this->field) {
        case self::CONSUMO_MEDIO:
          return $this->calculatePartidaConsumoMedio($lista);
          break;
        case self::TIEMPO_TOTAL:
          return $this->calculatePartidaTiempoTotalMedio($lista);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado para el tipo ListaPartidas.");
          break;
      }
    }
    else if ($lista instanceof ListaDatosInstantaneos) {
      switch ($this->field) {
        case self::VELOCIDAD:
          return $this->calculateVelocidadMediaPartida($lista);
          break;
        case self::RPM:
          return $this->calculateRpmMediaPartida($lista);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado para el tipo ListaDatosInstantaneos.");
          break;
      }
    }
    else {
      throw new Exception("La lista pasada no es de un tipo soportado");
    }
  }

  /**
   * @param ListaPartidas $lista
   * @return float Consumo medio entre todas las partidas de la lista pasada.
   */
  private function calculatePartidaConsumoMedio(ListaPartidas $lista) {
    $resultadoConsumoMedio = 0;

    if ($lista->count() > 0) {
      $consumoMedio = 0;

      foreach ($lista as $partida) {
        $consumoMedio += $partida->getConsumoMedio();
      }

      $resultadoConsumoMedio = $consumoMedio / $lista->count();
    }

    return $resultadoConsumoMedio;
  }

  /**
   * @param ListaPartidas $lista
   * @return float Tiempo total entre todas las partidas de la lista pasada.
   */
  private function calculatePartidaTiempoTotalMedio(ListaPartidas $lista) {
    $resultadoTiempoTotalMedio = 0;

    if ($lista->count() > 0) {
      $tiempoTotalMedio = 0;

      foreach ($lista as $partida) {
        $tiempoTotalMedio += $partida->getTiempoTotal();
      }

      $resultadoTiempoTotalMedio = $tiempoTotalMedio / $lista->count();
    }

    return $resultadoTiempoTotalMedio;
  }

  /**
   * @param ListaDatosInstantaneos $lista
   * @return float Velocidad media de una partida.
   */
  private function calculateVelocidadMediaPartida(ListaDatosInstantaneos $lista) {
    $resultadoVelocidadTotal = 0;

    if ($lista->count() > 0) {
      $velocidadTotal = 0;

      foreach ($lista as $dato) {
        $velocidadTotal += $dato->getVelocidad();
      }

      $resultadoVelocidadTotal = $velocidadTotal / $lista->count();
    }

    return $resultadoVelocidadTotal;
  }

  /**
   * @param ListaDatosInstantaneos $lista
   * @return float RPMs medias de una partida.
   */
  private function calculateRpmMediaPartida(ListaDatosInstantaneos $lista) {
    $resultadoRpmTotal = 0;

    if ($lista->count() > 0) {
      $velocidadRpm = 0;

      foreach ($lista as $dato) {
        $velocidadRpm += $dato->getRpm();
      }

      $resultadoRpmTotal = $velocidadRpm / $lista->count();
    }

    return $resultadoRpmTotal;
  }
}