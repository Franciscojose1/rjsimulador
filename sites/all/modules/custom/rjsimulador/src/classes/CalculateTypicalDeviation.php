<?php

/**
 * Class CalculateTypicalDeviation Clase que permite calcular la desviación típica de los datos de la lista recibida.
 */
class CalculateTypicalDeviation implements CalculatedDataInterface{
  const VELOCIDAD = 0;
  const RPM = 1;

  private $field;

  /**
   * @param int $field Debe ser una de las constantes de la clase CalculateTypicalDeviation.
   * @see CalculatedDataInterface
   * @see CalculateAverageData
   */
  public function __construct($field) {
    $this->field = $field;
  }

  /**
   * @param Lista $lista
   * @return mixed Devuelve el cálculo del campo de los elementos de la lista pasada.
   * @throws Exception Si el campo pasado no es admisible para el tipo de lista o si la lista no es un tipo soportado.
   */
  public function calculate(Lista $lista) {
    if ($lista instanceof ListaDatosInstantaneos) {
      switch ($this->field) {
        case self::VELOCIDAD:
          return $this->calculateTypicaDeviationVelocidad($lista);
          break;
        case self::RPM:
          return $this->calculateTypicalDeviationRpm($lista);
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

  private function calculateTypicaDeviationVelocidad(ListaDatosInstantaneos $lista) {
    $desviacionTipicaVelocidad = 0;

    if ($lista->count() > 0) {
      // Calculamos la velocidad media de los datos
      $velocidadMedia = $lista->calculateData(new CalculateAverageData(CalculateAverageData::VELOCIDAD));

      // Calculamos la suma de los cuadrados de la diferencia velocidad con velocidad media
      $sumaCuadradosVelocidad = 0;
      foreach ($lista as $dato) {
        $sumaCuadradosVelocidad += pow(($dato->getVelocidad() - $velocidadMedia), 2);
      }

      $totalDatos = $lista->count() > 1 ? ($lista->count() - 1) : 1;

      $desviacionTipicaVelocidad = sqrt($sumaCuadradosVelocidad / $totalDatos);
    }

    return $desviacionTipicaVelocidad;
  }

  private function calculateTypicalDeviationRpm(ListaDatosInstantaneos $lista) {
    $desviacionTipicaRpm = 0;

    if ($lista->count() > 0) {
      // Calculamos las rpm media de los datos
      $rpmsMedia = $lista->calculateData(new CalculateAverageData(CalculateAverageData::RPM));

      // Calculamos la suma de los cuadrados de la diferencia rpm con rpm media
      $sumaCuadradosRpm = 0;
      foreach ($lista as $dato) {
        $sumaCuadradosRpm += pow(($dato->getRpm() - $rpmsMedia), 2);
      }

      $totalDatos = $lista->count() > 1 ? ($lista->count() - 1) : 1;

      $desviacionTipicaRpm = sqrt($sumaCuadradosRpm / $totalDatos);
    }

    return $desviacionTipicaRpm;
  }
}