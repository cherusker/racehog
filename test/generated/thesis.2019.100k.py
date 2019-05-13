class Config(object):

    def __init__(self, num_threads, num_layers, num_children_per_func):
        self.num_threads           = num_threads
        self.num_layers            = num_layers
        self.num_children_per_func = num_children_per_func

    @property
    def num_threads(self):
        return self.__num_threads

    @num_threads.setter
    def num_threads(self, num):
        self.__num_threads = num

    @property
    def num_layers(self):
        return self.__num_layers

    @num_layers.setter
    def num_layers(self, num):
        self.__num_layers = num

    @property
    def num_children_per_func(self):
        return self.__num_children_per_func

    @num_children_per_func.setter
    def num_children_per_func(self, num):
        self.__num_children_per_func = num

def get_func_name(func_id, layer_id):
    return "fn{}_{}".format(layer_id, func_id)

def gen_call(func_id, layer_id):
    print("  (void) {}(glob_val, &val);".format(get_func_name(func_id, layer_id)))

def gen_juggle_val(val_name):
    print("  val += *{}_val;".format(val_name))
    print("  for (int i = 0; i < 55; ++i) {")
    print("    int x = val;")
    print("    x *= 2;")
    print("    val += x;")
    print("  }")
    print("  *{}_val = val;".format(val_name))
    pass

def gen_func(config, func_id, layer_id):
    print("void {}(int* glob_val, int* parent_val) {{".format(get_func_name(func_id, layer_id)))
    print("  int val = 0;")
    gen_juggle_val("glob")
    for i in range(7):
        gen_juggle_val("parent")
    if layer_id < config.num_layers - 1:
        base_child_id  = func_id * config.num_children_per_func
        child_layer_id = layer_id + 1
        for i in range(config.num_children_per_func):
            gen_call(base_child_id + i, child_layer_id)
        if func_id > 0: # Additionally, call a child of the previous function in this layer.
            gen_call((func_id - 1) * config.num_children_per_func, child_layer_id)
    print("}")

def gen_layer(config, layer_id):
    for func_id in range(pow(config.num_children_per_func, layer_id)):
        gen_func(config, func_id, layer_id)

def gen_thread():
    print("void* thread(void* glob_val) {")
    print("  int val = 0;")
    print("  (void) {}((int*)glob_val, &val);".format(get_func_name(0, 0)))
    print("  return NULL;")
    print("}")

def gen_main(config):
    print("int main(void) {")
    print("  int val;")
    for tid in range(config.num_threads):
        print("  pthread_t t{};".format(tid))
        print("  (void) pthread_create(&t{}, NULL, thread, &val);".format(tid))
    for tid in range(config.num_threads):
        print("  (void) pthread_join(t{}, NULL);".format(tid))
    print("  return EXIT_SUCCESS;")
    print("}")

def gen_header():
    print("#include <pthread.h>")
    print("#include <stdlib.h>")

def gen_code(config):
    gen_header()
    for i in reversed(range(config.num_layers)):
        gen_layer(config, i)
    gen_thread()
    gen_main(config)

if __name__ == "__main__":
    gen_code(Config(2, 5, 6))
