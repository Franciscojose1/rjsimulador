<?php

class FilterByEquality implements FilterInterface {
  /* Campos por los que filtrar */
  const USUARIO_ID = "uid";
  const USUARIO_USUAL_PLAYER = "user usual player";
  const INFRACCION_ID = "infraccion id";
  const DATO_MARCHA = "dato marcha";

  /* @var string $field */
  private $field;
  /* @var array $data */
  private $data;

  public function __construct(array $paramData, $paramField) {
    foreach ($paramData as $data) {
      if (is_array($data) || is_object($data)) {
        throw new InvalidArgumentException("Los datos pasados tienen que ser datos únicos, no arrays ni objetos.");
      }
    }

    if (!isset($paramField)) {
      throw new InvalidArgumentException("Es necesario el tipo de dato por el que comprobar.");
    }

    $this->data = $paramData;
    $this->field = $paramField;
  }

  /**
   * @param mixed $item
   * @return bool Devuelve si el item el id pertenece al array de ids pasados.
   * @throws Exception Si el elemento es de un tipo no soportado por el filtro.
   */
  public function filter($item) {
    if ($item instanceof UsuarioSimulacion) {
      switch ($this->field) {
        case self::USUARIO_ID:
          return $this->filterUsuarioById($item);
          break;
        case self::USUARIO_USUAL_PLAYER:
          return $this->filterUsuarioByFieldUsualPlayer($item);
          break;
        default:
          throw new Exception("No se puede procesar el campo pasado (" . $this->field . ") para el tipo UsuarioSimulacion.");
          break;
      }
    }
    else {
      if ($item instanceof Infraccion) {
        switch ($this->field) {
          case self::INFRACCION_ID:
            return $this->filterInfraccionByID($item);
            break;
          default:
            throw new Exception("No se puede procesar el campo pasado (" . $this->field . ") para el tipo Infraccion.");
            break;
        }
      }
      else {
        if ($item instanceof DatoInstantaneo) {
          switch ($this->field) {
            case self::DATO_MARCHA:
              return $this->filterDatoByMarcha($item);
              break;
            default:
              throw new Exception("No se puede procesar el campo pasado (" . $this->field . ") para el tipo DatoInstantaneo.");
              break;
          }
        }
        else {
          throw new Exception("El item pasado no es un tipo soportado.");
        }
      }
    }
  }

  private function filterUsuarioById(UsuarioSimulacion $item) {
    foreach ($this->data as $id) {
      if ($id == $item->getUid()) {
        return TRUE;
      }
    }

    return FALSE;
  }

  private function filterUsuarioByFieldUsualPlayer(UsuarioSimulacion $item) {
    foreach ($this->data as $usualPlayer) {
      if ($usualPlayer == 1 && $item->isUsualVideogamePlayer()) {
        return TRUE;
      }

      if ($usualPlayer == 0 && !$item->isUsualVideogamePlayer()) {
        return TRUE;
      }
    }

    return FALSE;
  }

  private function filterInfraccionByID(Infraccion $item) {
    foreach ($this->data as $id) {
      if ($id == $item->getIdInfraccion()) {
        return TRUE;
      }
    }

    return FALSE;
  }

  public function filterDatoByMarcha(DatoInstantaneo $item) {
    foreach ($this->data as $marcha) {
      if ($item->getMarcha() == $marcha) {
        return TRUE;
      }
    }

    return FALSE;
  }
}