import java.sql.SQLOutput;
import java.util.Observable;

public class Alien extends Observable {
    private Integer posX;
    private Integer posY;
    private Integer speed;
    private final Game game;


    public Alien(Game game, Integer posX, Integer posY, Integer speed) {
        this.posX = posX;
        this.posY = posY;
        this.speed = speed;
        this.game = game;
    }

    public Integer getPosX() {
        return posX;
    }

    public Integer getPosY() {
        return posY;
    }

    public Integer getSpeed() {
        return speed;
    }


    public void change() {
        this.speed *= -1;
        this.posY += 10;
        this.posX += this.speed;
    }

    public void move() {
        if (this.posX >= 1200) {
            this.change();
            if(!this.game.reachedRightCorner) {
                setChanged();
                notifyObservers("move");
                this.game.setReachedRightCorner();
            }
        }
        if (this.posX <= 0) {
            this.change();
            if(this.game.reachedRightCorner) {
                //this.change();
                setChanged();
                notifyObservers("move");
                this.game.setReachedRightCorner();
            }
        }else {
            this.posX += this.speed;
        }
    }
}
