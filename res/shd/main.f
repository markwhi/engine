#version 330

in vec2 o_texc;

uniform int c_mode = -1;
uniform sampler2D tex;
uniform vec4 c;

out vec4 out_c;

void main(){
    if(c_mode == 1){
        out_c = c;
    }else{
        vec4 sample_c = texture(tex, o_texc);

        if(c_mode == -1){
            out_c = sample_c;
        }else{
            out_c = sample_c * c;
        }
    }
}
