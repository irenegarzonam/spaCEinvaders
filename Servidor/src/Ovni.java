import java.util.Random;

public class Ovni {
    Integer posx;
    Integer posy;
    Integer speed;
    boolean exists;

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

    public Ovni() {
        this.exists = false;
    }

    public void move() {
        this.posx += this.speed;
        if(this.posx <= 0 || this.posx >= 1250){
            this.exists = false;
        }
    }
    public static Integer getRandomInteger() {
        Random rand = new Random();
        Integer randomInt = rand.nextInt(2); // Generates either 0 or 1
        return (randomInt == 0 ? -1 : 1);
    }

    public Integer getPosx() {
        return posx;
    }

    public Integer getPosy() {
        return posy;
    }
}
