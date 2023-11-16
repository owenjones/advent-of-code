(require '[clojure.string :as str])
(def opcodes (map read-string (str/split (slurp "test_input.txt") #",")))

; take 4, inspect first, perform action on second and third, use fourth for index
(defn processOpcodes [quartet]
  (let [result (case (first quartet)
                 1 (+ (second quartet) (nth quartet 2))
                 2 (* (second quartet) (nth quartet 2)))])
  ((= (first quartet) 99)))

