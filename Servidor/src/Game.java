import java.net.ServerSocket;
import java.sql.SQLOutput;
import java.util.*;

public class Game implements Observer {
    private Integer score;
    private Integer lives;
    private Alien[][] aliens;
    private List<Bunker> bunkers;
    private Ovni ovni;
    private ServerSocket server;
    boolean activeGame;
    boolean reachedRightCorner;

    public Game(ServerSocket server) {
        score = 0;
        lives = 2;
        aliens = new Alien[5][12];
        bunkers = new ArrayList<Bunker>();
        this.server = server;
        this.activeGame = true;
        this.reachedRightCorner = false;
        this.ovni = new Ovni();
        createAliens(5, 12, 55, 75, 15);
        createBunkers();
    }

    public String updateGame() {
            boolean flag = this.reachedRightCorner;
            // Actualizar la posición de los extraterrestres, los ovnis y los disparos
            // Verifica si algun alien llego al final de la pantalla
            for (Integer i = 0; i < aliens.length; i++) {
                for (Integer j = 0; j < aliens[0].length; j++) {
                    if(flag == this.reachedRightCorner) {
                        if (aliens[i][j] != null) {
                            aliens[i][j].move();
                            if (aliens[i][j].getPosY() >= 700) {//1000 es el screen Height
                                lives--;
                                aliens[i][j].deleteObserver(this);
                                aliens[i][j] = null;
                            }
                        }
                    }
                }
                if (!this.ovni.exists && getRandomNumber() % 5 == 0) {
                    createOvni();
                }
                if (this.ovni.exists) {
                    this.ovni.move();
                }
            }
            verifyWin();
            try {
                Thread.sleep(1000); // waits for 1 second
            } catch (InterruptedException e) {
            }
            //String clientAnswer = this.server.receiveMessage();
            return generateFinalString();
        }


    void setReachedRightCorner(){
        this.reachedRightCorner = !this.reachedRightCorner;
    }

    void getAlienCoords(String message){
        String[] parts = message.split("_");
        String type = parts[0];
        if(type == "calamar"){
            this.score += 10;
        }else if(type == "pulpo"){
            this.score += 40;
        }else if(type == "cangrejo"){
            this.score += 20;
        }
        String[] nums = parts[1].split(",");
        int x = Integer.parseInt(nums[0]);
        int y = Integer.parseInt(nums[1]);
        List<Integer> alienCoords = findPosAlien(x,y);
        deleteAlien(alienCoords.get(0), alienCoords.get(1));
    }

    public void createAliens(Integer numRows, Integer numCols, Integer x, Integer y, Integer speed){
        for (Integer i = 0; i < numRows; i++) {
            for (Integer j = 0; j < numCols; j++) {
                this.aliens[i][j] = new Alien(this, x + j * 80, y + i * 80, speed);
                this.aliens[i][j].addObserver(this);
            }
        }
    }

    public List<Integer> findPosAlien(Integer xPos, Integer yPos) {
        List<Integer> intList = new ArrayList<>();
        for (Integer i = 0; i < aliens.length; i++) {
            for (Integer j = 0; j < aliens[0].length; j++) {
                if(this.aliens[i][j].getPosX() == xPos && this.aliens[i][j].getPosY() == yPos){
                    intList.add(i);
                    intList.add(j);
                    return intList;
                }
            }
        }
        return intList;
    }

    public void deleteAlien(Integer Row, Integer Column) {
        aliens[Row][Column].deleteObserver(this);
        aliens[Row][Column] = null;
    }

    public void createBunkers() {
        for (Integer i = 1; i <= 4; i++) {
            Bunker newBunker = new Bunker((1100 / 4) * i - 75, 500);
            bunkers.add(newBunker);
        }
    }

    public void createOvni(){
        this.ovni = new Ovni(8);
    }

    @Override
    public void update(Observable o, Object arg) {
        if (arg instanceof String && arg.equals("move")) {
            updateSpeed();
        }
    }

    private void updateSpeed() {
        for (int row = 0; row < aliens.length; row++) {
            for (int col = 0; col < aliens[0].length; col++) {
                //if(aliens[row][col] != null) {
                    aliens[row][col].change();
                //}
            }
        }
    }

    public static Integer getRandomNumber() {
        Random rand = new Random();
        Integer randomInt = rand.nextInt(25) + 1; // Generates a random number between 1 and 25
        return randomInt;
    }

    public void verifyWin() {
        boolean win = true;
        for (Integer i = 0; i < aliens.length; i++) {
            for (Integer j = 0; j < aliens[0].length; j++) {
                if (aliens[i][j] != null) {
                    win = false;
                    break;
                }
            }
        }
        if (win) {
            this.lives += 1;
            this.score += 100;
            //this.activeGame = false;
        }
    }

    String generateAliensMatrix() {
        Integer matrix[][][] = new Integer[aliens.length][aliens[0].length][2];
        System.out.println("{");
        for (int row = 0; row < aliens.length; row++) {
            System.out.println();
            for (int col = 0; col < aliens[0].length; col++) {
                if(aliens[row][col] != null) {
                    matrix[row][col][0] = this.aliens[row][col].getPosX();
                    matrix[row][col][1] = this.aliens[row][col].getPosY();
                    System.out.print("{" + matrix[row][col][0] + "," + matrix[row][col][1] + "}");
                }else{
                    break;
                }
            }
        }
        System.out.println("}");
        String matrixAsString = Arrays.deepToString(matrix);
        System.out.println(matrixAsString);
        return matrixAsString;
    }

    String generateBunkersMatrix() {
        String Bunkers = "[";
        //int n = 0;
        for (Integer i = 0; i < bunkers.size(); i++) {
            Integer x = bunkers.get(i).getPosX();
            Integer y = bunkers.get(i).getPosY();
            Integer health = bunkers.get(i).getHealth();
            String add = "[" + Integer.toString(x) + "," + Integer.toString(y) + "," + Integer.toString(health) + "],";
            Bunkers += add;
            //n = i;
        }
        Bunkers += "]";
        //System.out.println(Bunkers);
        return Bunkers;
    }

    String generateOvniString() {
        String Ovni = "[" + this.ovni.getPosx() + "," + this.ovni.getPosy() + "]";
        return Ovni;
    }

    String generateFinalString() {
        String Score = Integer.toString(this.score);
        String Lives = Integer.toString(this.lives);
        String FinalString = Score + "_" + Lives + "_" + generateBunkersMatrix() + "_" + generateAliensMatrix()+"_"+generateOvniString();
        //System.out.println();
        //System.out.print(FinalString);
        //System.out.println();
        return FinalString;
    }
}
