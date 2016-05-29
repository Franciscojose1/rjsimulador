<?php
namespace Drupal\rjsimulador;

use stdClass;
use DateTime;
use Exception;
use Drupal\rjsimulador\Factory\FactoryDataManager;
use Drupal\rjsimulador\Filters\FilterByEquality;
use Drupal\rjsimulador\ListUtils\ListaSimulaciones;
use Drupal\rjsimulador\ListUtils\ListaPartidas;
use Drupal\rjsimulador\ListUtils\ListaInfracciones;

/**
 * Class UsuarioSimulacion Wrapper des la entidad user de Drupal para este módulo.
 */
class UsuarioSimulacion {
  /* @var stdClass $user La entidad usuario */
  private $user;
  /* @var ListaSimulaciones $listaSimulaciones Lista de simulaciones del usuario */
  private $listaSimulaciones;

  public function __construct(stdClass $user) {
    $this->setUser($user);
  }

  /**
   * @return stdClass
   */
  private function getUser() {
    return $this->user;
  }

  /**
   * @param stdClass $user
   */
  private function setUser($user) {
    $this->user = $user;
  }

  /**
   * @return ListaSimulaciones
   */
  public function getListaSimulaciones() {
    if (!isset($this->listaSimulaciones)) {
      $provider = FactoryDataManager::createDataProvider();
      $this->listaSimulaciones = $provider->loadListaSimulacionesByUsuario($this->getUid());
    }
    return $this->listaSimulaciones;
  }

  /**
   * @return int El UID del usuario.
   */
  public function getUid() {
    return $this->getUser()->uid;
  }

  /**
   * @return string
   */
  public function getName() {
    return $this->getUser()->name;
  }

  /**
   * @return string
   */
  public function getMail() {
    return $this->getUser()->mail;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de creación como DateTime o como Timestamp.
   */
  public function getCreationDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->created) : $this->getUser()->created;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de login como DateTime o como Timestamp.
   */
  public function getLoginDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->login) : $this->getUser()->login;
  }

  /**
   * @param bool $asDate Si queremos la fecha como Timestamp o como un objeto DateTime.
   * @return DateTime|int La fecha de útlimo acceso como DateTime o como Timestamp.
   */
  public function getLastAccessDate($asDate = FALSE) {
    return $asDate ? (new DateTime())->setTimestamp($this->getUser()->access) : $this->getUser()->access;
  }

  /**
   * Devuelve la edad del usuario en función de su fecha de nacimiento.
   * @return int La edad del usuario.
   */
  public function getAge() {
    $birthdate = (new DateTime())->setTimestamp($this->getUser()->field_birthdate['und'][0]['value']);
    $now = new DateTime();
    $interval = $now->diff($birthdate);
    return $interval->y;
  }

  /**
   * Devuelve los años de experiencia de conducción del usuario.
   * @return int Los años de experiencia de conducción del usuario.
   */
  public function getDrivingExperience() {
    $dateDrivingLicense = (new DateTime())->setTimestamp($this->getUser()->field_driving_experience['und'][0]['value']);
    $now = new DateTime();
    $interval = $now->diff($dateDrivingLicense);
    return $interval->y;
  }

  /**
   * Devuelve la media de kilómetros anuales que realiza el usuario.
   * @return int La media de km. anuales.
   */
  public function getAverageAnnualMileage() {
    return $this->getUser()->field_average_annual_mileage['und'][0]['value'];
  }

  /**
   * Devuelve si el usuario juega habitualmente a los videojuegos.
   * @param bool $asBoolean Si se pone a TRUE se devuelve el resultado como un booleano en lugar de con 1 y 0.
   * @return bool|int Devuelve 1 si el usuario es un jugador habitual o 0 en caso contrario.
   */
  public function isUsualVideogamePlayer($asBoolean = FALSE) {
    if ($asBoolean) {
      return $this->getUser()->field_usual_videogame_player['und'][0]['value'] == 1;
    }
    else {
      return $this->getUser()->field_usual_videogame_player['und'][0]['value'];
    }
  }

  /**
   * Devuelve el array de datos para el filtro por intervalos del grupo de edad al que pertenece el usuario.
   * @param bool $asText Si está a TRUE se devuelve el grupo como una cadena en lugar de como un array.
   * @return array|string Datos del grupo de edad del usuario como array o cadena.
   * @throws \Exception
   */
  public function getGrupoEdad($asText = FALSE) {
    foreach (Grupos::getGruposEdad() as $idGrupo => $arrayDatos) {
      if ($this->getAge() >= $arrayDatos['desde'] && $this->getAge() < $arrayDatos['hasta']) {
        if ($asText) {
          return t("age from " . $arrayDatos['desde'] . " to " . $arrayDatos['hasta'] . " years");
        }
        else {
          return $arrayDatos;
        }
      }
    }

    throw new Exception("El usuario debe estar dentro de un grupo de edad obligatoriamente. Revise los datos de la clase Grupos.");
  }

  /**
   * Devuelve el array de datos para el filtro por intervalos del grupo de experiencia conductora al que pertenece el usuario.
   * @param bool $asText Si está a TRUE se devuelve el grupo como una cadena en lugar de como un array.
   * @return array|string Datos del grupo de experiencia del usuario como array o cadena.
   * @throws \Exception
   */
  public function getGrupoExperiencia($asText = FALSE) {
    foreach (Grupos::getGruposExperiencia() as $idGrupo => $arrayDatos) {
      if ($this->getDrivingExperience() >= $arrayDatos['desde'] && $this->getDrivingExperience() < $arrayDatos['hasta']) {
        if ($asText) {
          return t("driving experience from " . $arrayDatos['desde'] . " to " . $arrayDatos['hasta'] . " years");
        }
        else {
          return $arrayDatos;
        }
      }
    }

    throw new Exception("El usuario debe estar dentro de un grupo de experiencia conductora obligatoriamente. Revise los datos de la clase Grupos.");
  }

  /**
   * Devuelve el array de datos para el filtro por intervalos del grupo de kilometraje medio anual al que pertenece el usuario.
   * @param bool $asText Si está a TRUE se devuelve el grupo como una cadena en lugar de como un array.
   * @return array|string Datos del grupo de kilometraje medio anual del usuario como array o cadena.
   * @throws \Exception
   */
  public function getGrupoKilometrajeMedioAnual($asText = FALSE) {
    foreach (Grupos::getGruposKmMedioAnual() as $idGrupo => $arrayDatos) {
      if ($this->getAverageAnnualMileage() >= $arrayDatos['desde'] && $this->getAverageAnnualMileage() < $arrayDatos['hasta']) {
        if ($asText) {
          return t("average annual mileage from " . $arrayDatos['desde'] . " to " . $arrayDatos['hasta'] . " years");
        }
        else {
          return $arrayDatos;
        }
      }
    }

    throw new Exception("El usuario debe estar dentro de un grupo de kilometraje anual medio obligatoriamente. Revise los datos de la clase Grupos.");
  }

  /**
   * Método que devuelve todas las partidas de un usuario.
   * @return ListaPartidas Lista de todas las partidas de ese usuario de todas las simulaciones.
   */
  public function retrieveAllPartidas() {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaSimulaciones() as $simulacion) {
      $listaPartidas->mergeList($simulacion->getListaPartidas());
    }

    return $listaPartidas;
  }

  /**
   * Método que devuelve todas las partidas del usuario para una simulación en concreto.
   * @param int $idSimulation El id de la simulación para la que recuperar las partidas de los usuarios.
   * @return ListaPartidas Lista de todas las partidas de esa simulación..
   */
  public function retrieveAllPartidasByIdSimulacion($idSimulation) {
    $listaPartidas = new ListaPartidas();

    foreach ($this->getListaSimulaciones() as $simulacion) {
      if ($simulacion->getIdSimulacion() == $idSimulation) {
        $listaPartidas->mergeList($simulacion->getListaPartidas());
        break;
      }
    }

    return $listaPartidas;
  }

  /**
   * Devuelve todas las infracciones de todas las partidas del usuario una simulación en concreto.
   * @param int $idSimulacion ID de la Simulación para la que recuperar las infracciones.
   * @return ListaInfracciones Lista de infracciones para la simulación pasada.
   */
  public function retrieveAllInfraccionesByIdSimulacion($idSimulacion) {
    $listaInfracciones = new ListaInfracciones();
    // Recuperamos todas las infracciones de una simulación en concreto
    foreach ($this->retrieveAllPartidasByIdSimulacion($idSimulacion) as $partida) {
      $listaInfracciones->mergeList($partida->getListaInfracciones());
    }

    return $listaInfracciones;
  }

  /**
   * Devuelve todas las infracciones de un cierto tipo de todas las partidas del usuario para la simulación pasada.
   * @param int $idInfraccion El ID de la infracción a recuperar.
   * @param int $idSimulacion ID de la simulación para la que recuperar las infracciones.
   * @return ListaInfracciones Lista de infracciones del tipo pasado para la simulación.
   */
  public function retrieveAllInfraccionesByTypeAndIdSimulacion($idInfraccion, $idSimulacion) {
    $listaInfracciones = $this->retrieveAllInfraccionesByIdSimulacion($idSimulacion);
    $arrayIdsInfracciones = array($idInfraccion);
    // Devolvemos la lista de infracciones filtradas por el IdInfraccion
    return $listaInfracciones->filterBy(new FilterByEquality($arrayIdsInfracciones, FilterByEquality::INFRACCION_ID));
  }

  /**
   * Devuelve la media de infracciones de un cierto tipo cometidas por partida del usuario para la simulación pasada.
   * @param int $idInfraccion El ID de la infracción.
   * @param int $idSimulation El ID dela simulación.
   * @return float|int Media de infracciones por partida.
   */
  public function getAverageInfraccionesByPartida($idInfraccion, $idSimulation) {
    // Recuperamos total de partidas de una simulacion
    $totalPartidasSimulacion = $this->retrieveAllPartidasByIdSimulacion($idSimulation)->count();
    // Recuperamos total de infracciones de un ciertto tipo por simulacion
    $totalInfraccionesSimulacion = $this->retrieveAllInfraccionesByTypeAndIdSimulacion($idInfraccion, $idSimulation)->count();

    // Si el total de partidas es 0 devolvemos 0 evitando la división
    $mediaInfraccionesPorPartida = $totalPartidasSimulacion > 0 ? $totalInfraccionesSimulacion/$totalPartidasSimulacion : 0;

    return $mediaInfraccionesPorPartida;
  }

  /**
   * @return int Número de partidas de este usuario.
   */
  public function countPartidas() {
    $numeroPartidas = 0;

    foreach ($this->getListaSimulaciones() as $simulacion) {
      $numeroPartidas += $simulacion->getListaPartidas()->count();
    }

    return $numeroPartidas;
  }
}