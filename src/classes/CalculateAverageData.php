<?php

class CalculateAverageData implements CalculatedDataInterface {
  const CONSUMO_MEDIO = 0;
  const TIEMPO_TOTAL = 1;

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
}