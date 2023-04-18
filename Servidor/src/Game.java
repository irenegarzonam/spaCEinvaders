import java.util.*;

public class Game implements Observer {
    private Integer score;
    private Integer lives;
    private Alien[][] aliens;
    private List<Bunker> bunkers;
    private Server server;
    boolean activeGame;

    public Game(Server server) {
        score = 0;
        lives = 2;
        aliens = new Alien[5][12];
        bunkers = new ArrayList<Bunker>();
        this.server = server;
        this.activeGame = true;
        createAliens(5, 12, 55, 75, 2);
        createBunkers();
    }

    public void update() {
        // Actualizar la posición de los extraterrestres, los ovnis y los disparos
        // Verifica si algun alien llego al final de la pantalla
        for (Integer i = 0; i < aliens.length; i++) {
            for (Integer j = 0; j < aliens[0].length; j++) {
                aliens[i][j].move();
                if (aliens[i][j].getPosY() >= 700) {//1000 es el screen Height
                    lives--;
                    aliens[i][j].deleteObserver(this);
                    aliens[i][j] = null;
                }
            }
        }
        verifyWin();
        this.server.sendToClient(generateFinalString());
        if(this.activeGame){
            this.update();
        }
    }

    public void createAliens(Integer numRows, Integer numCols, Integer x, Integer y, Integer speed) {
        for (Integer i = 0; i < numRows; i++) {
            for (Integer j = 0; j < numCols; j++) {
                this.aliens[i][j] = new Alien(x + j * 80, y + i * 80, speed);
                this.aliens[i][j].addObserver(this);
            }
        }
        generateAliensMatrix();
    }

    public void deleteAlien(Integer Row, Integer Column) {
        aliens[Row][Column].deleteObserver(this);
        aliens[Row][Column] = null;
    }

    public void createBunkers() {
        for (Integer i = 1; i <= 4; i++) {
            Bunker newBunker = new Bunker((1250 / 4) * i - 75, 500);
            bunkers.add(newBunker);
        }
    }

    @Override
    public void update(Observable o, Object arg) {
        if (arg instanceof String && arg.equals("speed")) {
            updateSpeed();
        } else if (arg instanceof String && arg.equals("posY")) {
            updatePosY();
        }
    }

    private void updateSpeed() {
        for (int row = 0; row < aliens.length; row++) {
            for (int col = 0; col < aliens[0].length; col++) {
                aliens[row][col].changeSpeed();
            }
        }
    }

    private void updatePosY() {
        for (int row = 0; row < aliens.length; row++) {
            for (int col = 0; col < aliens[0].length; col++) {
                aliens[row][col].changePosY();
            }
        }
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
        int n = 0;
        for (Integer i = 0; i < bunkers.size(); i++) {
            Integer x = bunkers.get(i).getPosX();
            Integer y = bunkers.get(i).getPosY();
            Integer health = bunkers.get(i).getHealth();
            String add = "[" + Integer.toString(x) + "," + Integer.toString(y) + "," + Integer.toString(health) + "],";
            Bunkers += add;
            n = i;
        }
        Bunkers += "]";
        System.out.println(Bunkers);
        return Bunkers;
    }

    String generateFinalString() {
        String Score = Integer.toString(this.score);
        String Lives = Integer.toString(this.lives);
        String FinalString = Score + "_" + Lives + "_" + generateBunkersMatrix() + "_" + generateAliensMatrix();
        System.out.println();
        System.out.print(FinalString);
        System.out.println();
        return FinalString;
    }
}
