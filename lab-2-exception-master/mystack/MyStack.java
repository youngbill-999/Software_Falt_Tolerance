public class MyStack {
    private int counter;
    private Object[] stack;

    public MyStack(int size) {
        counter = -1;
        stack = new Object[size];
    }

    /**
     * @return number of elements on MyStack.
     */
    public int getCounter() { return counter + 1; }

    /**
     * @return String representation of the Object
     */
    public String toString() {
        String s = new String();
        for(int i = 0; i <= counter; i++) {
            s = s.concat(stack[i] + ";;");
        }
        return s;
    }

    /**
     * This method is used to add an object on the MyStack.
     *
     * @param p the object to put on the MyStack
     */
    public void addElement(Object p) {
        try { 
            counter++;
            stack[counter] = p;
        }
        catch(ArrayIndexOutOfBoundsException e) {
        System.out.println("Out of Bound!");
        resizeStack();
        stack[counter] = p;
        }
        
    }

    /**
     * This method is used to resize the MyStack. It is useful if <i>addElement</i> returns an <i>ArrayIndexOutOfBoundsException</i>.
     */
    public void resizeStack() {
        int newSize = stack.length + 3;
        Object[] newStack = new Object[newSize];
        System.arraycopy(stack, 0, newStack, 0, stack.length);
        stack = newStack;
 }

}