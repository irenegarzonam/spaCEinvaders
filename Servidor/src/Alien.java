import java.sql.SQLOutput;
import java.util.Observable;

/**
 * Clase Alien
 */
public class Alien extends Observable {
    private Integer posX;
    private Integer posY;
    private Integer speed;
    private final Game game;

    /**
     * Metodo constructor del alien
     * @param game recibe el juego en el que se va a encontrar
     * @param posX posicion en x en la que va a aparecer el alien
     * @param posY posicion en y en la que va a aparecer el alien
     * @param speed velocidad a la que se va a mover el alien
     */
    public Alien(Game game, Integer posX, Integer posY, Integer speed) {
        this.posX = posX;
        this.posY = posY;
        this.speed = speed;
        this.game = game;
    }

    /**
     * Obtiene la posicion en x del alien
     * @return posicion en x
     */
    public Integer getPosX() {
        return posX;
    }

    /**
     * Obtiene la posicion en y del alien
     * @return posicion en y
     */
    public Integer getPosY() {
        return posY;
    }

    /**
     * Cambios en y y la velocidad del alien al colisionar con uno de los lados de la pantalla
     */
    public void change() {
        this.speed *= -1;
        this.posY += 10;
        this.posX += this.speed;
    }

    /**
     * Movimiento del Alien
     */
    public void move() {
        if (this.posX >= 1200) {
            this.change();
            if(!this.game.reachedRightCorner) {
                setChanged();
                notifyObservers("move");
                this.game.setReachedRightCorner();
            }
        }else if (this.posX <= 0) {
            this.change();
            if(this.game.reachedRightCorner) {
                setChanged();
                notifyObservers("move");
                this.game.setReachedRightCorner();
            }
        }else {
            this.posX += this.speed;
        }
    }

    /**
     * Cambia la posicion en y del alien
     * @param posY Nueva posicion y del alien
     */
    public void setPosY(Integer posY) {
        this.posY = posY;
    }
}
