/**
 * Clase del bunker de juego
 */
class Bunker {
    private final Integer posX;
    private final Integer posY;
    private Integer health;

    /**
     * Metodo constructor del bunker
     * @param posX posicion en x del bunker
     * @param posY posicion en y del bunker
     */
    public Bunker(Integer posX, Integer posY) {
        this.posX = posX;
        this.posY = posY;
        this.health = 100;
    }

    /**
     * Obtiene la posicion en x del bunker
     * @return posicion en x del bunker
     */
    public Integer getPosX() {
        return posX;
    }

    /**
     * Obtiene la posicion en y del bunker
     * @return posicion en y del bunker
     */
    public Integer getPosY() {
        return posY;
    }

    /**
     * Obtiene la vida del bunker
     * @return vida del bunker
     */
    public Integer getHealth() {
        return health;
    }

    /**
     * Le baja la vida al bunker cuando recibe un golpe
     */
    public void gotHit(){
        this.health -= 25;
    }


}