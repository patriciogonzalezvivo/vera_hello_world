#include "vera/app.h"
#include "vera/ops/string.h"
#include "vera/ops/meshes.h"

using namespace std;
using namespace vera;
using namespace glm;

class VeraApp : public App {
    
    vec3            satellite;
    vector<vec3>    orbit;
    Light           sun;

    void setup() {

        // Set global settings
        background(0.0f);
        blendMode(BLEND_ALPHA);

        // Set camera
        camera()->setPosition( vec3(0.0f, 0.0f, 10.0f) );
        camera()->lookAt( vec3(0.0f, 0.0f, 0.0f) );
        camera()->setViewport( (int)width, (int)height );
        pixelDensity( std::max(1.0, std::min(width, height) / 1200.0) * getDisplayPixelRatio());

        // Set light
        sun.setPosition( vec3(1.0f,1.0f,1.0f) );
        sun.setType(LIGHT_POINT);
        addLight("sun", sun);

        // Set world shader and texture
        string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            uniform sampler2D   u_tex0;
            uniform vec3        u_light;

            #ifdef MODEL_VERTEX_NORMAL
            varying vec3        v_normal;
            #endif

            #ifdef MODEL_VERTEX_TEXCOORD
            varying vec2        v_texcoord;
            #endif

            void main () {
                vec3 color = vec3(1.);

                #ifdef MODEL_VERTEX_TEXCOORD
                color = vec3( 0.5 + texture2D(u_tex0, v_texcoord).r * 0.5);
                #endif
                
                #ifdef MODEL_VERTEX_NORMAL
                float shade = dot(v_normal, normalize(u_light));
                shade = smoothstep(-0.25, 0.25, shade);
                color *= 0.2 + shade * 0.8;
                #endif

                gl_FragColor = vec4(color, 1.);
            }
        )";
        addShader("world", createShader(frag));
        addTexture("earth-water", "earth-water.png");

        // Set satellite label
        addLabel("Beep beep", &satellite, LABEL_LINE_TO_WINDOW_BORDER, 10.0f);        
    }

    void update() {
        if (frameCount%15 == 0) {
            orbit.push_back( satellite );
            if (orbit.size() > 500)
                orbit.erase(orbit.begin());
        }
    }

    void draw() {
        clear(0.0f);

        setDepthTest(true);
        orbitControl();

        sun.setPosition( vec3(cos(frameCount * 0.01f), 0.0, sin(frameCount * 0.01f)) );

        fill(1.0f);
        noStroke();
        push();
        rotateY(frameCount * 0.0025f);
        Shader* world_shader = shader("world");
        world_shader->setUniform("u_light", sun.getPosition());
        world_shader->setUniformTexture("u_tex0", texture("earth-water"));
        sphere(1.0f);
        pop();
        
        resetShader();

        push();
        rotateY(frameCount * 0.0035f);
        rotateX(frameCount * 0.005f);
        translate(0.0f,0.0f,1.2f);
        fill(0.75f + sin(millis() * 0.005f) * 0.25f, 0.0f, 0.0f);
        box(0.075f);
        satellite = vec3( worldMatrix() * vec4(0.0f, 0.0f, 1.2f, 0.0f) );
        pop();

        strokeWeight(1.0);
        stroke(0.75f, 0.0f, 0.0f);
        line(orbit);

        textAlign(ALIGN_CENTER);
        textAlign(ALIGN_MIDDLE);
        textSize(28.0f);
        fill(1.0f);
        text("Hello World", width * 0.5f, height * 0.95f);

        textSize(18.0f);
        stroke(1.0f);
        labels();
    }

    void onWindowResize(int _width, int _height) {
        camera()->setViewport(_width, _height);
        pixelDensity( std::max(1.0, std::min(_width, _height) / 1200.0) * getDisplayPixelRatio());
    }
};

VeraApp app;

int main(int argc, char **argv) {
    vera::WindowProperties prop;
    prop.screen_width = 1080/2;
    prop.screen_height = 1920/2;

    #if !defined(__EMSCRIPTEN__) && !defined(PLATFORM_RPI)
    prop.msaa = 4;
    #endif

    app.run(prop);
    return 1;
}
