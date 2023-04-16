import java.util.*;

public class Alien extends Observable {
    private Integer posX;
    private Integer posY;
    private Integer speed;

    private boolean updatingSpeed = false;

    public Alien(Integer posX, Integer posY, Integer speed) {
        this.posX = posX;
        this.posY = posY;
        this.speed = speed;
    }

    public Integer getPosX() {
        return posX;
    }

    public Integer getPosY() {
        return posY;
    }

    public void changePosY() {
        this.posY += 10;
        setChanged();
        notifyObservers("posY");
    }

    public void changeSpeed() {
        if (!updatingSpeed) { // check if updatingSpeed flag is set
            updatingSpeed = true;
            speed *= -1;
            setChanged();
            notifyObservers("speed");
            updatingSpeed = false;
        }
    }

    public void move() {
        if(this.posX <= 0 || this.posX >= 1250){
            changeSpeed();
            changePosY();
        }
        this.posX += this.speed;
    }
}
