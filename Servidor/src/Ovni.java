import java.util.Random;

/**
 * Clase ovni
 */
public class Ovni {
    Integer posx;
    Integer posy;
    Integer speed;
    boolean exists;

    /**
     * Metodo constructor del ovni
     * @param speed Unicamente recibe la velocidad a la que se mueve el ovni
     */
    public Ovni(Integer speed) {
        this.speed = speed * getRandomInteger();
        this.posy = 30;
        this.exists = true;
        if(this.speed < 0){
            this.posx = 1250;
        }
        else{
            this.posx = 0;
        }
    }

    /**
     * Metodo constructor sin paramentros
     */
    public Ovni() {
        this.exists = true;
    }

    /**
     * Cambia la posicion en x del ovni para que se mueva
     */
    public void move() {
        this.posx += this.speed;
        if(this.posx <= 0 || this.posx >= 1250){
            this.exists = false;
        }
    }

    /**
     * Produce un numero random de -1 o 1 para decidir en que direccion se va a mover el ovni
     * @return 1 o -1
     */
    public static Integer getRandomInteger() {
        Random rand = new Random();
        Integer randomInt = rand.nextInt(2); // Generates either 0 or 1
        return (randomInt == 0 ? -1 : 1);
    }

    /**
     * Obtiene la posicion en x del ovni
     * @return posicion en x
     */
    public Integer getPosx() {
        return posx;
    }

    /**
     * Obtiene la posicion en y del ovni
     * @return posicion en y
     */
    public Integer getPosy() {
        return posy;
    }
}
