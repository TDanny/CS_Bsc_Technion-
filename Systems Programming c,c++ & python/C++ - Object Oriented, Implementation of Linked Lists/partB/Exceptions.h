#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

namespace mtm
{

    /**
     * Exception class is a polymorphism of std::exception which we added more exception according to *pdf instructions.
    */ 
    class Exceptions: public std::exception
    {
        private:
        const char *message;
        public:


        /**
         *Exceptions::Exceptions -  constructor 
         * @param error - the massege -char* type -  that provided according to specific problem that accord in the game.
        */
        Exceptions(const char* error):message(error){};


        /**
         *what() -  method which returns the error massage 
         * @return:
         * const char* - the error which is saved in message variable.
        */
        const char* what() const noexcept override 
        {
            return message;
        }
    };


    /**
     * IllegalCell class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class IllegalCell : public Exceptions
    {
        public:
        IllegalCell():Exceptions("A game related error has occurred: IllegalCell"){};

    };

    /**
     * IllegalArgument class is a polymorphism of Exceptions which characteristics by *pdf.
     */
        class IllegalArgument : public Exceptions
    {
        public:
        IllegalArgument():Exceptions("A game related error has occurred: IllegalArgument"){};

    };

    /**
     * CellEmpty class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class CellEmpty : public Exceptions
    {
        public:
        CellEmpty():Exceptions("A game related error has occurred: CellEmpty"){};

    };


    /**
     * MoveTooFar class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class MoveTooFar : public Exceptions
    {
        public:
        MoveTooFar():Exceptions("A game related error has occurred: MoveTooFar"){};

    };


    /**
     * CellOccupied class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class CellOccupied : public Exceptions
    {
        public:
        CellOccupied():Exceptions("A game related error has occurred: CellOccupied"){};

    };


    /**
     * OutOfRange class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class OutOfRange : public Exceptions
    {
        public:
        OutOfRange():Exceptions("A game related error has occurred: OutOfRange"){};

    };


    /**
     * OutOfAmmo class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class OutOfAmmo : public Exceptions
    {
        public:
        OutOfAmmo():Exceptions("A game related error has occurred: OutOfAmmo"){};

    };


    /**
     * IllegalTarget class is a polymorphism of Exceptions which characteristics by *pdf.
     */
    class IllegalTarget : public Exceptions
    {
        public:
        IllegalTarget():Exceptions("A game related error has occurred: IllegalTarget"){};
    };
}

#endif //EXCEPTIONS_H_